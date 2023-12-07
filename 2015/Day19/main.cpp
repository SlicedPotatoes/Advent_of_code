/*
  L'entrée représente une grammaire, un peu comme dans l'algorithme CYK (Cocke-Younger-Kasami)
  À la fin, une chaîne de caractères est fournie sur laquelle nous allons travailler.

  Dans la Partie 1, l'objectif est de compter le nombre de combinaisons possibles sur la chaîne d'entrée en fonction de
  la grammaire.
  Exemple avec la chaîne "HOH":

  La grammaire de l'entrée:
  e => H
  e => O
  H => HO
  H => OH
  O => HH

  Il y a 5 combinaisons possibles après une modification de la chaîne d'origine, dont 4 uniques :
  HOOH (H => HO sur le premier H)
  HOHO (H => HO sur le second H)
  OHOH (H => OH sur le premier H)
  HOOH (H => OH sur le second H)
  HHHH (O => HH sur le premier O)

  Dans la Partie 2, l'objectif est de compter le nombre d'étapes nécessaires en partant de la chaîne "e" pour arriver à
  la chaîne d'entrée.

  Par exemple, en reprenant la grammaire d'entrée ci-dessus, il nous faudrait 3 étapes en partant de "e" pour arriver à
  "HOH":
  [e => O]  => O
  [O => HH] => HH
  [H => HO] => HOH
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

map<string, vector<string>> mapReplacements;
string input;

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
    while (getline(file, line))
    {
        if (line == "")
        {
            getline(file, input);
            continue;
        }
        vector<string> splitedInput = splitString(line, " => ");
        mapReplacements[splitedInput[0]].push_back(splitedInput[1]);
    }
    file.close();
    return 0;
}

int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Utiliser un ensemble pour stocker les chaînes uniques résultant des remplacements
    set<string> uniqueString;

    // Parcourir les remplacements dans la mapReplacements
    for (auto &it : mapReplacements)
    {
        // Trouver la première occurrence de la clé dans la chaîne d'entrée
        for (string s : it.second)
        {
            // Trouver la première occurrence de la clé dans la chaîne d'entrée
            size_t pos = input.find(it.first);

            // Effectuer les remplacements dans la chaîne d'entrée
            while (pos != string::npos)
            {
                string newString = input.substr(0, pos) + s + input.substr(pos + it.first.size());

                // Insérer la nouvelle chaîne dans l'ensemble pour garantir l'unicité
                uniqueString.insert(newString);

                // Trouver la prochaine occurrence de la clé dans la chaîne d'entrée
                pos = input.find(it.first, pos + 1);
            }
        }
    }

    return uniqueString.size();
}
vector<string> alreadySee;

// Effectue un parcours récursif pour atteindre la molécule cible "e" à partir de la molécule actuelle.
// Le paramètre 'count' représente le nombre d'étapes effectuées.
int backtrack(string molecule, int count)
{
    // Vérifier si la molécule a déjà été traitée pour éviter une boucle infinie
    if (find(alreadySee.begin(), alreadySee.end(), molecule) != alreadySee.end())
    {
        return -1;
    }
    // Vérifier si la molécule est la cible finale "e"
    if (molecule == "e")
    {
        return count;
    }

    // Ajouter la molécule actuelle à la liste des molécules déjà vues
    alreadySee.push_back(molecule);

    // Parcourir les remplacements possibles dans la mapReplacements
    for (auto &it : mapReplacements)
    {
        // Parcourir les chaînes de remplacement associées à la clé actuelle
        for (string s : it.second)
        {
            // Trouver la première occurrence de la chaîne de remplacement dans la molécule
            size_t pos = molecule.find(s);

            // Effectuer les remplacements dans la molécule
            while (pos != string::npos)
            {
                string newString = molecule.substr(0, pos) + it.first + molecule.substr(pos + s.size());

                // Récursivement appeler backtrack avec la nouvelle molécule et le compte mis à jour
                int value = backtrack(newString, count + 1);

                // Si une solution est trouvée, la retourner
                if (value != -1)
                {
                    return value;
                }

                // Trouver la prochaine occurrence de la chaîne de remplacement dans la molécule
                pos = molecule.find(s, pos + 1);
            }
        }
    }

    // Aucune solution trouvée, retourner -1
    return -1;
}

int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Appeler la fonction backtrack pour effectuer le parcours récursif
    return backtrack(input, 0);
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processP1();
    int part2 = processP2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}