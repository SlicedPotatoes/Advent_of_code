/*
  La première ligne de l'entrée représente une liste d'instructions.

  Le reste représente les deux chemins suivants pour un chemin.

  Pour parcourir un chemin depuis un départ jusqu'à une arrivée, il faut suivre les instructions.
  Si on arrive à la fin des instructions, on reprend du début jusqu'à atteindre l'arrivée.
  L'instruction "L" signifie prendre le chemin de gauche et l'instruction "R" signifie prendre le chemin de droite.

  Pour la partie 1, l'objectif est de compter le nombre d'étapes nécessaires en partant du chemin "AAA" jusqu'au chemin
  "ZZZ".

  Pour la partie 2, on part de tous les chemins qui finissent par "A". L'objectif est de trouver le nombre d'étapes
  nécessaires pour arriver à un chemin finissant par "Z" sur tous les chemins en même temps.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

string instructions;
map<string, vector<string>> nodes;

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

    getline(file, instructions);

    while (getline(file, line))
    {
        if (line == "")
        {
            continue;
        }
        vector<string> splitedInput = splitString(line, " = ");
        splitedInput[1] = splitedInput[1].substr(1);
        splitedInput[1].pop_back();
        vector<string> nodeChild = splitString(splitedInput[1], ", ");

        nodes[splitedInput[0]] = nodeChild;
    }
    file.close();

    return 0;
}

// Fonction pour la Partie 1: Compter le nombre d'étapes nécessaires pour aller de "AAA" à "ZZZ"
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation des variables
    int nbStep = 0;
    int indexInstruction = 0;
    string actualNode = "AAA";

    // Boucle tant que la destination n'est pas atteinte
    while (actualNode != "ZZZ")
    {
        // Suivre l'instruction "L" pour le chemin de gauche, sinon "R" pour le chemin de droite
        if (instructions[indexInstruction] == 'L')
        {
            actualNode = nodes[actualNode][0];
        }
        else
        {
            actualNode = nodes[actualNode][1];
        }

        // Incrémentation du nombre d'étapes
        nbStep++;

        // Passage à l'instruction suivante
        indexInstruction++;

        // Si on atteint la fin de la liste d'instructions, revenir au début
        if (indexInstruction == instructions.size())
        {
            indexInstruction = 0;
        }
    }

    // Retourner le nombre total d'étapes
    return nbStep;
}

// Fonction pour trouver le nombre minimum d'étapes pour atteindre un chemin finissant par "Z" depuis un chemin initial
// donné
int findMinStep(string start)
{
    // Initialisation des variables
    int nbStep = 0;
    int indexInstruction = 0;

    // Boucle tant que la destination "Z" n'est pas atteinte
    while (start[2] != 'Z')
    {
        // Suivre l'instruction "L" pour le chemin de gauche, sinon "R" pour le chemin de droite
        if (instructions[indexInstruction] == 'L')
        {
            start = nodes[start][0];
        }
        else
        {
            start = nodes[start][1];
        }

        // Incrémentation du nombre d'étapes
        nbStep++;

        // Passage à l'instruction suivante
        indexInstruction++;

        // Si on atteint la fin de la liste d'instructions, revenir au début
        if (indexInstruction == instructions.size())
        {
            indexInstruction = 0;
        }
    }

    // Retourner le nombre total d'étapes nécessaires pour atteindre "Z"
    return nbStep;
}

// Fonction pour calculer le Plus Grand Commun Diviseur (PGCD) de deux nombres
long long pgcd(long long a, long long b)
{
    // Vecteurs pour stocker les diviseurs de chaque nombre
    vector<long long> _a;
    vector<long long> _b;

    // Trouver le plus petit des deux nombres
    long long minValue = min(a, b);

    // Trouver les diviseurs du premier nombre
    for (long long i = 1; i <= minValue; i++)
    {
        if (a % i == 0)
        {
            _a.push_back(i);
        }
    }

    // Trouver les diviseurs du deuxième nombre
    for (long long i = 1; i <= minValue; i++)
    {
        if (b % i == 0)
        {
            _b.push_back(i);
        }
    }

    // Initialiser la valeur maximale commune à zéro
    long long max = 0;

    // Parcourir les diviseurs des deux nombres pour trouver le plus grand commun
    for (long long elementA : _a)
    {
        for (long long elementB : _b)
        {
            if (elementA == elementB && elementA > max)
            {
                max = elementA;
            }
        }
    }

    // Retourner le PGCD trouvé
    return max;
}

// Fonction pour la Partie 2 : Trouver le nombre minimum d'étapes pour atteindre un chemin finissant par "Z" depuis ceux
// finissant par "A"
long long processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Vecteur pour stocker le nombre minimum d'étapes pour chaque chemin finissant par "A"
    vector<int> minSteps;

    // Parcourir tous les chemins
    for (auto &it : nodes)
    {
        // Sélectionner les chemins finissant par "A"
        if (it.first[2] == 'A')
        {
            // Trouver le nombre minimum d'étapes pour chaque chemin
            minSteps.push_back(findMinStep(it.first));
        }
    }

    // Trier le vecteur pour obtenir le plus grand nombre minimum d'étapes en dernier
    sort(minSteps.begin(), minSteps.end());

    // Initialiser le résultat avec le plus grand nombre minimum d'étapes
    long long result = minSteps.back();

    // Calcul du plus petit commun multiple (PPCM) des nombres minimums d'étapes
    for (int index = minSteps.size() - 2; index >= 0; index--)
    {
        long long a = pgcd(result, minSteps[index]);
        result = (result * minSteps[index]) / pgcd(result, minSteps[index]);
    }

    // Retourner le résultat final
    return result;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();
    long long part2 = processPart2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}