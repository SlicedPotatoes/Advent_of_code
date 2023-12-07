/*
  L'entrée représente l'état initial d'une grille de 100x100 pour le "Jeu de la vie".

  Les règles du jeu de la vie sont les suivantes :
  - Une cellule morte possédant exactement 3 cellules voisines vivantes devient vivante.
  - Une cellule vivante possédant 2 ou 3 cellules voisines vivantes reste vivante.
  - Si une cellule ne respecte aucune règle citée plus haut, elle meurt.

  Partie 1 : L'objectif est de compter le nombre de cellules vivantes après 100 étapes.

  Partie 2 : Les cellules dans les coins restent toujours vivantes. L'objectif est le même que pour la partie 1.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

struct Point
{
    int x, y;
};

const int MAP_SIZE = 100;
const int NB_STEP = 100;
const vector<Point> dir = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

bool default_map[MAP_SIZE][MAP_SIZE];
bool map[MAP_SIZE][MAP_SIZE];

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
    int y = 0;
    while (getline(file, line))
    {
        // Parcours de chaque caractère dans la ligne
        for (int x = 0; x < line.size(); x++)
        {
            // Initialisation de la carte par défaut en fonction du caractère rencontré
            if (line[x] == '#')
            {
                default_map[y][x] = true; // Cellule vivante
            }
            else
            {
                default_map[y][x] = false; // Cellule morte
            }
        }
        y++;
    }
    file.close();
    return 0;
}
// Passer à l'étape suivante du "Jeu de la vie"
void nextStep(bool isP2)
{
    // Création d'une carte temporaire pour stocker le prochain état
    bool tempMap[MAP_SIZE][MAP_SIZE];

    // Parcours de chaque cellule dans la carte actuelle
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            // Comptage du nombre de voisins vivants
            int countNeighbor = 0;
            for (Point d : dir)
            {
                int dx = x + d.x;
                int dy = y + d.y;
                if (dy >= 0 && dy < MAP_SIZE && dx >= 0 && dx < MAP_SIZE && map[dy][dx])
                {
                    countNeighbor++;
                }
            }

            // Application des règles du "Jeu de la vie"
            if (map[y][x] && (countNeighbor == 2 || countNeighbor == 3))
            {
                tempMap[y][x] = true; // La cellule reste vivante
            }
            else if (!map[y][x] && countNeighbor == 3)
            {
                tempMap[y][x] = true; // Une cellule morte devient vivante
            }
            else
            {
                tempMap[y][x] = false; // La cellule meurt
            }
        }
    }

    // Partie 2 : Les cellules dans les coins restent toujours allumées
    if (isP2)
    {
        tempMap[0][0] = true;
        tempMap[0][99] = true;
        tempMap[99][0] = true;
        tempMap[99][99] = true;
    }

    // Copie du contenu de la carte temporaire dans la carte principale
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            map[y][x] = tempMap[y][x];
        }
    }
}

int process(bool isP2)
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation de la carte avec l'état par défaut
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            map[y][x] = default_map[y][x];
        }
    }

    // Partie 2 : Les cellules dans les coins restent toujours allumées
    if (isP2)
    {
        map[0][0] = true;
        map[0][99] = true;
        map[99][0] = true;
        map[99][99] = true;
    }

    // Simulation des étapes successives du "Jeu de la vie"
    for (int i = 0; i < NB_STEP; i++)
    {
        nextStep(isP2);
    }

    // Comptage du nombre de cellules vivantes après les étapes spécifiées
    int count = 0;
    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            if (map[y][x])
            {
                count++;
            }
        }
    }

    // Retourne le nombre de cellules vivantes
    return count;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = process(false);
    int part2 = process(true);

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}