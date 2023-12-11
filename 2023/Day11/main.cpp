/*
  L'entrée représente une grille:
  - Les cases '.' sont vides.
  - Les cases '#' sont des galaxies.

  Étant donné que l'univers s'étend, chaque colonne / ligne ne contenant que des cases vides est répétée 1 fois pour la
  partie 1 ou 1 000 000 de fois pour la partie 2. Cela signifie que l'on rajoute 1 ou 999999 (1M - 1) en fonction de la
  partie traitée à l'emplacement de la colonne/ligne.

  Exemple

  Input:
  ...#......
  .......#..
  #.........
  ..........
  ......#...
  .#........
  .........#
  ..........
  .......#..
  #...#.....

  Carte après application de la règle décrite plus haut : Les '*' représentent les lignes vides qui ont été doublées
  (Pour la partie 1)
  ..**#.**..**.
  ..**..**.#**.
  #.**..**..**.
  *************
  *************
  ..**..**#.**.
  .#**..**..**.
  ..**..**..**#
  *************
  *************
  ..**..**.#**.
  #.**.#**..**.


  L'objectif des 2 parties est de calculer la somme des distances entre chaque galaxie en tenant compte de l'expansion
  de l'univers.
*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

const char GALAXY = '#';
const char EMPTY_CELL_EXTEND = '*';
const int EXTENDS_RATIO = 1000000 - 1;

// Structure représentant un point dans la grille
struct Point
{
    int x, y;
};

vector<string> inputs;

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
        inputs.push_back(line);
    }
    file.close();

    // Remplissage des lignes vides dans la grille
    for (int y = 0; y < inputs.size(); y++)
    {
        bool haveGalaxy = false;

        // Vérification si la ligne contient une galaxie
        for (int x = 0; x < inputs[0].size(); x++)
        {
            if (inputs[y][x] == GALAXY)
            {
                haveGalaxy = true;
                break;
            }
        }

        // Si la ligne ne contient pas de galaxie, remplacer par EMPTY_CELL_EXTEND
        if (!haveGalaxy)
        {
            string fill(inputs[0].size(), EMPTY_CELL_EXTEND);
            inputs[y] = fill;
        }
    }

    // Remplissage des colonnes vides dans la grille
    for (int x = 0; x < inputs[0].size(); x++)
    {
        bool haveGalaxy = false;

        // Vérification si la colonne contient une galaxie
        for (int y = 0; y < inputs.size(); y++)
        {
            if (inputs[y][x] == GALAXY)
            {
                haveGalaxy = true;
                break;
            }
        }

        // Si la colonne ne contient pas de galaxie, remplacer par EMPTY_CELL_EXTEND
        if (!haveGalaxy)
        {
            for (int y = 0; y < inputs.size(); y++)
            {
                string fill(1, EMPTY_CELL_EXTEND);
                inputs[y].replace(x, 1, fill);
            }
        }
    }

    // Affichage de la grille après traitement
    for (string s : inputs)
    {
        cout << s << '\n';
    }

    return 0;
}

// Calcul de la distance de Manhattan entre deux points
int distanceManhattan(Point p1, Point p2)
{
    return abs(p2.x - p1.x) + abs(p2.y - p1.y);
}

// Obtention des coordonnées des galaxies dans la grille étendue
vector<Point> getGalaxies(int n)
{
    vector<Point> galaxies;

    int rowExtends = 0;

    // Parcours des lignes de la grille
    for (int y = 0; y < inputs.size(); y++)
    {
        int colExtends = 0;

        // Si la première cellule de la ligne est une extension, ajuster rowExtends
        if (inputs[y][0] == EMPTY_CELL_EXTEND)
        {
            rowExtends += n;
        }

        // Parcours des colonnes de la grille
        for (int x = 0; x < inputs[0].size(); x++)
        {
            // Si la cellule actuelle est une extension, ajuster colExtends
            if (inputs[y][x] == EMPTY_CELL_EXTEND)
            {
                colExtends += n;
            }
            // Si la cellule contient une galaxie, ajouter ses coordonnées à la liste des galaxies
            else if (inputs[y][x] == GALAXY)
            {
                galaxies.push_back({x + colExtends, y + rowExtends});
            }
        }
    }

    return galaxies;
}

// Traitement principal pour calculer la somme des distances entre les galaxies
long long process(int n)
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Obtention des coordonnées étendues des galaxies
    vector<Point> galaxies = getGalaxies(n);

    // Initialisation de la somme des distances
    long long sum = 0;

    // Calcul de la somme des distances entre chaque paire de galaxies
    for (int i = 0; i < galaxies.size(); i++)
    {
        for (int j = i + 1; j < galaxies.size(); j++)
        {
            sum += distanceManhattan(galaxies[i], galaxies[j]);
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

    long long part1 = process(1);
    long long part2 = process(EXTENDS_RATIO);

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}