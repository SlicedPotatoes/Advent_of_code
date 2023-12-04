/*
  L'entrée représente une liste de personnes avec un score d'affinité envers les autres personnes de la liste.

  Pour la partie 1, l'objectif est de trouver la combinaison qui maximise le bonheur global.
  Chaque personne sera assise à une table circulaire et aura exactement 2 voisins.

  Partie 2 : On remarque que nous avons omis de nous inclure dans la liste.
  L'objectif reste le même, mais en s'incluant dans la liste :
  - Notre affinité personnelle avec les autres invités est neutre.
  - On suppose que l'affinité des autres invités envers nous est également neutre.
*/

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

unordered_map<string, unordered_map<string, int>> input;

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
        // Supprime le dernier caractère (qui est un point '.')
        line = line.substr(0, line.size() - 1);

        // Divise la ligne en mots en utilisant l'espace comme délimiteur
        vector<string> splitedLine = splitString(line, " ");

        // Récupère les noms des personnes concernées
        string p1 = splitedLine[0];
        string p2 = splitedLine[10];

        // Calcule le bonheur en fonction du type ("gain" ou "lose") et de la valeur
        int happiness = splitedLine[2] == "gain" ? stoi(splitedLine[3]) : -stoi(splitedLine[3]);

        // Enregistre l'affinité dans la structure de données appropriée
        input[p1][p2] = happiness;
    }
    file.close();
    return 0;
}

// Calcule le bonheur global
int getGlobalHappiness(vector<string> &list)
{
    int happiness = 0;
    for (int i = 0; i < list.size(); i++)
    {
        string person = list[i];
        // Détermine la personne à droite (circularité)
        string right = i == 0 ? list[list.size() - 1] : list[i - 1];
        // Détermine la personne à gauche (circularité)
        string left = i == list.size() - 1 ? list[0] : list[i + 1];

        // Ajoute le bonheur de la personne avec ses voisins à la variable totale
        happiness += input[person][right];
        happiness += input[person][left];
    }
    return happiness;
}

int dfs(vector<string> &list)
{
    // Vérifie si la liste est complète (toutes les personnes sont incluses)
    if (list.size() == input.size())
    {
        return getGlobalHappiness(list);
    }

    int maxValue = 0;

    // Explore chaque personne non encore ajoutée à la liste
    for (auto &p1 : input)
    {
        if (find(list.begin(), list.end(), p1.first) == list.end())
        {
            // Ajoute la personne à la liste et récursivement explore les combinaisons
            list.push_back(p1.first);
            maxValue = max(maxValue, dfs(list));

            // Retire la dernière personne ajoutée pour explorer d'autres combinaisons
            list.pop_back();
        }
    }
    return maxValue;
}

// Fonction de traitement de la Partie 1
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Liste initiale vide
    vector<string> list;

    // Appelle la fonction de parcours en profondeur (DFS) pour obtenir le bonheur maximal
    return dfs(list);
}

// Fonction de traitement de la Partie 2
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Ajoute la personne "me" à la liste avec une affinité neutre envers tous les autres invités, et vice versa
    for (auto &p1 : input)
    {
        input["me"][p1.first] = 0;
        input[p1.first]["me"] = 0;
    }

    // Liste initiale vide
    vector<string> list;

    // Appelle la fonction de parcours en profondeur (DFS) pour obtenir le bonheur maximal
    return dfs(list);
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();
    int part2 = processPart2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}