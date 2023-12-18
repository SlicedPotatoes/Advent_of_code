/*
  L'entrée représente une liste d'instructions séparées par des virgules.

  L'objectif de la partie 1 est de trouver le hachage de chaque instruction et de retourner la somme pour l'ensemble de
  la liste.

  L'objectif de la partie 2 est de remplir des boîtes en suivant les instructions comme suit:
  - Chaque instruction est séparée en deux parties :
    - Un libellé, qui est l'ensemble des caractères qui précèdent un '-' ou un '='.
    - L'opération à effectuer.

  Exemples :
  - "rn=1" => libellé : "rn", opération : "=1"
  - "cm-"  => libellé : "cm", opération : "-"

  Nous disposons de 256 boîtes. En récupérant le hachage du libellé, nous obtenons l'indice de la boîte.
  Si l'opération est "-", nous devons supprimer de la boîte l'élément ayant le même libellé.
  Si l'opération est "=", nous devons ajouter à la boîte un élément dont la valeur est le chiffre suivant le '=', et si
  un élément porte le même libellé, nous remplaçons simplement sa valeur.

  Ensuite, nous devons calculer la somme des puissances de nos éléments:
  puissance = (boxID + 1) * (slot + 1) * valeur

  Le calcul du hachage se fait comme suit:
  - Nous initialisons notre hachage à 0.
  - Nous incrémentons la valeur par le code ASCII du caractère que nous traitons.
  - Nous multiplions la valeur par 17.
  - La valeur est égale au reste de la division par 256.
  - Nous répétons l'opération 2 jusqu'à 4 tant qu'il nous reste des caractères.
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

// Définition de la structure représentant un élément de boîte
struct BoxElement
{
    string label; // Libellé de l'élément
    int value;    // Valeur associée à l'élément
};

vector<string> inputs; // Vecteur stockant les instructions en entrée

int parseInput()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    string filename = "input.txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
        return 1;
    }
    string line;
    getline(file, line);

    inputs = splitString(line, ",");
    file.close();

    return 0;
}

// Fonction pour calculer le hachage d'une chaîne de caractères
int getHash(string &s)
{
    int currentValue = 0;

    for (char c : s)
    {
        int asciiCode = (int)c;

        currentValue += asciiCode;
        currentValue *= 17;
        currentValue %= 256;
    }

    return currentValue;
}

// Fonction pour traiter la Partie 1 du problème
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int sum = 0;

    for (string &s : inputs)
    {
        sum += getHash(s);
    }

    return sum;
}

// Fonction pour traiter la Partie 2 du problème
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation du vecteur de boîtes
    vector<vector<BoxElement>> box(256);

    for (string &s : inputs)
    {
        // Traitement pour l'opération "-"
        if (s[s.size() - 1] == '-')
        {
            string label = s.substr(0, s.size() - 1);
            int labelHash = getHash(label);

            // Recherche de l'élément dans la boîte
            auto it = find_if(box[labelHash].begin(), box[labelHash].end(),
                              [&label](const BoxElement &obj) { return obj.label == label; });

            // Suppression de l'élément s'il existe
            if (it != box[labelHash].end())
            {
                box[labelHash].erase(it);
            }
        }
        // Traitement pour l'opération "="
        else
        {
            string label = s.substr(0, s.size() - 2);
            int labelHash = getHash(label);
            int labelValue = stoi(s.substr(s.size() - 1));

            // Recherche de l'élément dans la boîte
            auto it = find_if(box[labelHash].begin(), box[labelHash].end(),
                              [&label](const BoxElement &obj) { return obj.label == label; });

            // Ajout de l'élément ou mise à jour de sa valeur
            if (it == box[labelHash].end())
            {
                box[labelHash].push_back({label, labelValue});
            }
            else
            {
                it.base()->value = labelValue;
            }
        }
    }

    // Calcul de la somme des puissances des éléments
    int sum = 0;

    for (int idBox = 0; idBox < box.size(); idBox++)
    {
        for (int index = 0; index < box[idBox].size(); index++)
        {
            sum += (idBox + 1) * (index + 1) * box[idBox][index].value;
        }
    }

    return sum;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();
    int part2 = processPart2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}