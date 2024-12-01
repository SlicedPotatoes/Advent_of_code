/*
  L'entrée représente deux listes de nombres.
  Pour chaque ligne, nous avons un élément de chacune des listes, séparés par des espaces.

  Partie 1:
  L'objectif est de trouver la somme des distances entre les i-ième plus petit élément des deux listes jusqu'à n.

  Partie 2:
  L'objectif est de déterminer la fréquence de chaque numéro de la liste de gauche apparaissant dans la liste de droite.
  Ensuite, nous devons calculer un score en additionnant chaque numéro de la liste de gauche multiplié par sa fréquence
  calculée précédemment.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

vector<int> list1;
vector<int> list2;

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
        vector<string> splitedLine = splitString(line, "   ");
        list1.push_back(stoi(splitedLine[0]));
        list2.push_back(stoi(splitedLine[1]));
    }
    file.close();
    return 0;
}

// Fonction pour traiter la partie 1 du problème
int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int res = 0;

    // Trie les deux listes pour accéder facilement aux ièmes plus petits éléments
    sort(list1.begin(), list1.end());
    sort(list2.begin(), list2.end());

    // Parcours des listes et calcul de la somme des distances
    for (int i = 0; i < list1.size(); i++)
    {
        res += abs(list1[i] - list2[i]);
    }

    return res;
}

// Fonction pour traiter la Partie 2 du problème
int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int res = 0;

    // Utilisation d'une hashmap pour calculer le nombre d'occurrences de chaque nombre dans la liste2
    unordered_map<int, int> mp;
    for (int n : list2)
    {
        mp[n]++;
    }

    // Parcours de list1 et calcul de la somme des éléments multipliés par leur fréquence
    for (int n : list1)
    {
        res += n * mp[n];
    }

    return res;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processP1();
    int part2 = processP2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}