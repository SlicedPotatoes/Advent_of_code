/*
    L'input représente le nombre de cadeaux.

    Partie 1: Trouver la maison la plus proche qui recevra au moins ce nombre de cadeaux, selon les règles suivantes:
    - Il existe une infinité d'elfes, chacun avec un numéro distinct (1, 2, 3, 4, etc.).
    - Il existe une infinité de maisons, chacune avec un numéro attribué (1, 2, 3, 4, etc.).
    - Chaque elfe donne 10 fois son numéro en cadeau.
    - Un elfe livre uniquement aux maisons où le reste de la division de son numéro est égal à 0. Exemple :

    Elfe 1: livre aux maisons 1, 2, 3, 4, etc., et donne 10 cadeaux.
    Elfe 2: livre aux maisons 2, 4, 6, 8, etc., et donne 20 cadeaux.
    Elfe 3 : livre aux maisons 3, 6, 9, 12, etc., et donne 30 cadeaux.

    Dans ces conditions, les premières maisons recevront:
    Maison 1: 10 cadeaux.
    Maison 2: 30 cadeaux (elfe 1 et 2).
    Maison 3: 40 cadeaux (elfe 1 et 3).
    Maison 4: 70 cadeaux (elfe 1, 2 et 4).
    Maison 5: 60 cadeaux (elfe 1 et 5).
    Maison 6: 120 cadeaux (elfe 1, 2, 3 et 6).
    Maison 7: 80 cadeaux (elfe 1 et 7).
    Etc.

    Partie 2: L'objectif est le même que pour la partie 1. Cependant, les règles ont changé :
    - Chaque elfe parcourt maintenant 50 maisons.
    - Chaque elfe donne 11 fois son numéro en cadeau.
*/
#include <fstream>
#include <iostream>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

int input;

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
    input = stoi(line);
    file.close();
    return 0;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Création d'un vecteur pour stocker les résultats par maison
    vector<int> elements(input + 1, 0);

    // Boucle sur les elfes
    for (int i = 1; i <= input; i++)
    {
        // Boucle sur les maisons
        for (int j = i; j <= input; j += i)
        {
            // Chaque elfe donne 10 fois son numéro en cadeau à la maison correspondante
            elements[j] += i * 10;
        }

        // Vérifie si la maison actuelle a reçu suffisamment de cadeaux
        if (elements[i] >= input)
        {
            return i;
        }
    }

    return -1;
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Création d'un vecteur pour stocker les résultats par maison
    vector<int> elements(input * 50 + 1, 0);

    // Boucle sur les elfes
    for (int i = 1; i <= input; i++)
    {
        // Chaque elfe parcourt maintenant 50 maisons
        for (int j = 1; j <= 50; j++)
        {
            int house = i * j;

            // Chaque elfe donne 11 fois son numéro en cadeau à la maison correspondante
            elements[house] += i * 11;
        }

        // Vérifie si la maison actuelle a reçu suffisamment de cadeaux
        if (elements[i] >= input)
        {
            return i;
        }
    }

    return -1;
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