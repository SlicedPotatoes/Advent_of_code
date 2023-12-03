/*
    L'entrée est du JSON.

    Partie 1 : Le but est de calculer la somme de tous les nombres présents dans ce JSON (les clés ne contiennent pas de
    nombres).

    Partie 2 : L'objectif demeure le même, cependant, si une valeur "red" est rencontrée dans un objet, l'objet en
    question  ainsi que ses enfants doivent être ignorés.
    Cette règle ne s'applique pas aux tableaux.

    Exemples :
    - [1, {"c": "red", "b": 2}, 3] a une somme de 4 car l'objet du milieu est ignoré.
    - {"d": "red", "e": [1,2,3,4], "f": 5} a une somme de 0.
    - [1, "red", 5] a une somme de 6, car "red" dans un tableau n'a aucun effet.


    Utilisation de la bibliothèque "JSON for Modern C++" pour parser et parcourir le JSON.
    https://github.com/nlohmann/json
*/

#include <fstream>
#include <iostream>
#include <string>

#include <Utils/Library/json.hpp>
#include <Utils/MeasureExecutionTime.cpp>

using namespace std;
using json = nlohmann::json;

json jsonObj;

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

    jsonObj = json::parse(line);
    file.close();

    return 0;
}

// Fonction DFS (Depth-First Search) pour calculer la somme des nombres dans un JSON en tenant compte des règles
// spécifiées.

// Paramètres :
// - jsonObject : Objet JSON à parcourir.
// - isP2 : Indique si nous sommes dans la Partie 2 du problème.
int dfs(const json &jsonObject, bool isP2)
{
    // Initialisation de la somme à 0.
    int sum = 0;

    // Parcours des éléments du JSON.
    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Vérification des règles de la Partie 2
        if (isP2 && jsonObject.is_object() && it->is_string())
        {
            string v = it.value();
            if (v == "red")
            {
                // Si une valeur "red" est trouvée dans un objet, on ignore l'objet et ses enfants.
                return 0;
            }
        }
        // Récursion pour les objets et les tableaux.
        if (it->is_object() || it->is_array())
        {
            sum += dfs(*it, isP2);
        }
        // Addition des nombres entiers.
        else if (it->is_number_integer())
        {
            int intValue = it.value();
            sum += intValue;
        }
    }
    return sum;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    return dfs(jsonObj, false);
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    return dfs(jsonObj, true);
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