/*
  L'entrée représente une liste de connexions entre villes et leurs distances.

  Partie 1 : Trouver le chemin le plus court passant par toutes les villes une seule fois.
  Partie 2 : Trouver le chemin le plus long passant par toutes les villes une seule fois.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

// Structure représentant la distance entre deux villes
struct CityDistance
{
    string from;  // Ville de départ
    string to;    // Ville de destination
    int distance; // Distance entre les deux villes
};
// Classe représentant un nœud dans le contexte du problème
class Node
{
  public:
    string actualCity;      // Ville actuelle du nœud
    int totalDistance;      // Distance totale parcourue jusqu'à ce nœud
    vector<string> history; // Historique des villes visitées jusqu'à ce nœud

    // Constructeur pour initialiser un nœud avec une nouvelle ville
    Node(string actualCity, string to, int totalDistance)
    {
        this->actualCity = to;
        this->totalDistance = totalDistance;
        history.push_back(actualCity);
        history.push_back(to);
    }

    // Surcharge de l'opérateur d'assignation
    Node &operator=(const Node &other)
    {
        actualCity = other.actualCity;
        totalDistance = other.totalDistance;
        history = other.history;
        return *this;
    }
};

// Table de hachage utilisant une unordered_map, associant chaque ville à une liste de distances vers d'autres villes
unordered_map<string, vector<CityDistance>> hashtable;

// Fonction pour analyser les données d'entrée à partir d'un fichier
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
        hashtable[splitedInput[0]].push_back({splitedInput[0], splitedInput[2], stoi(splitedInput[4])});
        hashtable[splitedInput[2]].push_back({splitedInput[2], splitedInput[0], stoi(splitedInput[4])});
    }
    file.close();

    return 0;
}
// Fonction de recherche en profondeur (DFS) pour la Partie 1 (distance minimale)
int dfsPart1(Node &n, int minDistance)
{
    // Si la distance totale dépasse la distance minimale, on retourne la distance minimale actuelle
    if (n.totalDistance > minDistance)
    {
        return minDistance;
    }
    // Si toutes les villes ont été visitées, on retourne la distance totale actuelle
    if (n.history.size() == hashtable.size())
    {
        return n.totalDistance;
    }

    // Parcours des connexions de la ville actuelle
    for (CityDistance cd : hashtable[n.actualCity])
    {
        // Vérification si la ville destination n'a pas déjà été visitée
        if (find(n.history.begin(), n.history.end(), cd.to) == n.history.end())
        {
            // Création d'une copie du nœud actuel avec la nouvelle ville visitée
            Node _n = n;
            _n.actualCity = cd.to;
            _n.totalDistance += cd.distance;
            _n.history.push_back(cd.to);

            // Appel récursif avec le nouveau nœud
            minDistance = min(minDistance, dfsPart1(_n, minDistance));
        }
    }

    return minDistance;
}
// Fonction de traitement pour la Partie 1
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int minDistance = INT_MAX;

    // Parcours de chaque ville comme point de départ
    for (auto &it : hashtable)
    {
        // Parcours des connexions de chaque ville
        for (CityDistance &cd : it.second)
        {
            Node n = {cd.from, cd.to, cd.distance};
            minDistance = dfsPart1(n, minDistance);
        }
    }

    return minDistance;
}
// Fonction de recherche en profondeur (DFS) pour la Partie 2 (distance maximale)
int dfsPart2(Node &n, int maxDistance)
{
    // Si toutes les villes ont été visitées
    if (n.history.size() == hashtable.size())
    {
        // on retourne la distance totale si elle est supérieure à la distance maximale
        if (n.totalDistance > maxDistance)
        {
            return n.totalDistance;
        }
        return maxDistance;
    }
    // Parcours des connexions de la ville actuelle
    for (CityDistance cd : hashtable[n.actualCity])
    {
        // Vérification si la ville destination n'a pas déjà été visitée
        if (find(n.history.begin(), n.history.end(), cd.to) == n.history.end())
        {
            // Création d'une copie du nœud actuel avec la nouvelle ville visitée
            Node _n = n;
            _n.actualCity = cd.to;
            _n.totalDistance += cd.distance;
            _n.history.push_back(cd.to);

            // Appel récursif avec le nouveau nœud
            maxDistance = max(maxDistance, dfsPart2(_n, maxDistance));
        }
    }

    return maxDistance;
}
// Fonction de traitement pour la Partie 2
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int maxDistance = -1;

    // Parcours de chaque ville comme point de départ
    for (auto &it : hashtable)
    {
        // Parcours des connexions de chaque ville
        for (CityDistance &cd : it.second)
        {
            Node n = {cd.from, cd.to, cd.distance};
            maxDistance = dfsPart2(n, maxDistance);
        }
    }

    return maxDistance;
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