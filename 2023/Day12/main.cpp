/*
 L'entrée représente une liste de lignes de nonogramme avec la définition de chaque ligne.
 Une ligne de nonogramme est composée de différents caractères:
 - '#' qui représente une case noire
 - '.' qui représente une case blanche
 - '?' qui représente une case non définie

 La définition d'une ligne est une liste de nombres qui représente le nombre de carrés noirs consécutifs.

 Par exemple, pour la définition "1, 1" d'une ligne qui fait 4 de longueur (????), on a les combinaisons suivantes
 possibles:
 - #.#.
 - .#.#
 - #..#

 L'objectif de la partie 1 est de compter, pour chaque ligne, le nombre de combinaisons et de retourner la somme de
 toutes les combinaisons.

 L'objectif de la partie 2 est le même, sauf que l'on doit répéter le motif d'origine 5 fois au total, séparé par un
 "?". Exemple:
 .# 1 devient:
 .#?.#?.#?.#?.# 1,1,1,1,1

*/
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const char UNKNOWN_SPRING = '?';
const char BROKEN_SPRING = '#';
const char FUNCTIONAL_SPRING = '.';

const vector<char> PROCESS_HELPER = {BROKEN_SPRING, FUNCTIONAL_SPRING};

struct Line
{
    string s;
    vector<int> controls; // Liste des nombres représentant le nombre de carrés noirs consécutifs
};

vector<Line> inputs; // Liste des lignes de nonogramme avec leur définition respective

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
        vector<string> splitedInput = splitString(line, " ");
        vector<string> stringControls = splitString(splitedInput[1], ",");
        vector<int> controls;

        for (string s : stringControls)
        {
            controls.push_back(stoi(s));
        }

        inputs.push_back({splitedInput[0], controls});
    }
    file.close();

    return 0;
}

unordered_map<string, long long> cache;
long long getNbCombinations(string s, vector<int> &d, size_t indexChar, size_t indexBlock, int current)
{
    string key = to_string(indexChar) + '_' + to_string(indexBlock) + '_' + to_string(current);

    // Vérification si la valeur est déjà en cache pour éviter le recalcul
    if (cache.find(key) != cache.end())
    {
        return cache[key];
    }

    // Cas de base : fin de la ligne atteinte
    if (indexChar == s.size())
    {
        // Si toutes les définitions ont été utilisées et le compteur est revenu à zéro, une combinaison est trouvée
        if (indexBlock == d.size() && current == 0)
        {
            return 1;
        }
        // Sinon, si la dernière définition est atteinte et le compteur correspond, une combinaison est trouvée
        else if (indexBlock == d.size() - 1 && current == d[indexBlock])
        {
            return 1;
        }
        // Aucune combinaison valide
        return 0;
    }

    long long count = 0;
    // Itération sur les caractères possibles pour chaque position dans la ligne
    for (char c : PROCESS_HELPER)
    {
        // Vérification si le caractère actuel de la ligne correspond à l'un des caractères possibles ou est indéfini
        if (s[indexChar] == c || s[indexChar] == UNKNOWN_SPRING)
        {
            // Cas où le caractère est une case blanche et le compteur est à zéro
            if (c == FUNCTIONAL_SPRING && current == 0)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock, 0);
            }
            // Cas où le caractère est une case blanche, le compteur est supérieur à zéro, et la définition correspond
            else if (c == FUNCTIONAL_SPRING && current > 0 && indexBlock < d.size() && d[indexBlock] == current)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock + 1, 0);
            }
            // Cas où le caractère est une case noire
            else if (c == BROKEN_SPRING)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock, current + 1);
            }
        }
    }
    // Mise en cache du résultat pour éviter de recalculer pour les mêmes paramètres
    cache[key] = count;
    return count;
}

// Fonction pour traiter la partie 1
long long processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    long long count = 0;

    // Itération sur chaque ligne d'entrée
    for (Line l : inputs)
    {
        // Appel de la fonction récursive pour chaque ligne
        count += getNbCombinations(l.s, l.controls, 0, 0, 0);

        // Effacer le cache après le traitement de chaque ligne
        cache.clear();
    }

    return count;
}

// Fonction pour traiter la partie 2
long long processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    long long count = 0;

    // Itération sur chaque ligne d'entrée
    for (Line l : inputs)
    {
        // Création d'une nouvelle ligne avec le motif répété cinq fois séparé par un '?'
        string s = l.s;
        vector<int> vec;
        for (int i = 0; i < 4; i++)
        {
            s += UNKNOWN_SPRING + l.s;
        }

        // Répétition des contrôles cinq fois
        for (int i = 0; i < 5; i++)
        {
            for (int element : l.controls)
            {
                vec.push_back(element);
            }
        }

        // Appel de la fonction récursive pour chaque ligne modifiée
        count += getNbCombinations(s, vec, 0, 0, 0);

        // Effacer le cache après le traitement de chaque ligne
        cache.clear();
    }

    return count;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    long long part1 = processPart1();
    long long part2 = processPart2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}