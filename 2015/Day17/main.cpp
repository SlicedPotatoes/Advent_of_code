/*
  L'entrée représente la capacité en litres du récipient.

  Partie 1: Trouver le nombre de combinaisons possibles uniques pour stocker 150 litres.

  Partie 2: Le même objectif que la Partie 1, sauf que l'on cherche le nombre de combinaisons possibles avec le minimum
  de conteneurs.
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

const int LIQUID_TO_STORE = 150;

vector<int> containers;

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
        containers.push_back(stoi(line));
    }
    file.close();
    return 0;
}
// Vector pour stocker les combinaisons uniques
vector<set<int>> uniqueCombinations;

// Backtracking pour trouver les combinaisons
int backtrack(vector<int> &_containers, int lastIndex)
{
    int totalCapacity = 0; // Capacité totale des contenants actuels

    // Calcul de la capacité totale des contenants actuels
    for (int idContainer : _containers)
    {
        totalCapacity += containers[idContainer];
    }

    // Vérification si la capacité totale est égale à la quantité de liquide à stocker
    if (totalCapacity == LIQUID_TO_STORE)
    {
        // Création d'un ensemble d'indices uniques pour les contenants actuels
        set<int> ids;
        for (int id : _containers)
        {
            ids.insert(id);
        }

        // Ajout de l'ensemble d'indices à la liste des combinaisons uniques
        uniqueCombinations.push_back(ids);
        return 1;
    }
    else if (totalCapacity > LIQUID_TO_STORE)
    {
        return 0; // La capacité totale dépasse la quantité de liquide à stocker, pas de combinaison valide
    }

    int count = 0;

    // Récursivité pour explorer toutes les combinaisons possibles
    for (int i = lastIndex; i < containers.size(); i++)
    {
        _containers.push_back(i);
        count += backtrack(_containers, i + 1);
        _containers.pop_back();
    }

    return count; // Retourne le nombre total de combinaisons trouvées
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Liste des indices des contenants utilisés dans une combinaison
    vector<int> _containers;

    // Appel de la fonction de backtracking pour trouver les combinaisons
    int result = backtrack(_containers, 0);

    return result;
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation des variables pour suivre le nombre minimum de contenants
    int min = INT_MAX;
    int countMin = 0;

    // Parcours des combinaisons uniques
    for (set<int> element : uniqueCombinations)
    {
        // Vérification si la combinaison actuelle a moins de conteneurs que le minimum actuel
        if (element.size() < min)
        {
            // Mise à jour du minimum et du compteur
            min = element.size();
            countMin = 1;
        }
        // Incrémentation du compteur si la combinaison a le même nombre de conteneurs que le minimum actuel
        else if (element.size() == min)
        {
            countMin++;
        }
    }

    // Retourne le nombre de combinaisons avec le minimum de contenants
    return countMin;
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
    cout << "Part2 : " << part2 << '\n';

    return 0;
}