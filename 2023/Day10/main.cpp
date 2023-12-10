/*
  L'entrée représente une grille remplie de tuyaux:

  | représente un tuyau vertical reliant le nord et le sud
  - représente un tuyau horizontal reliant l'est et l'ouest
  L est un virage à 90 degrés reliant le nord et l'est
  J est un virage à 90 degrés reliant le nord et l'ouest
  7 est un virage à 90 degrés reliant le sud et l'ouest
  F est un virage à 90 degrés reliant le sud et l'est
  . représente une case vide
  S est la position de départ, c'est un tuyau qui peut aller dans toutes les directions

  L'objectif de la partie 1 est de trouver le point le plus éloigné atteignable et de retourner le nombre d'étapes
  nécessaires pour y arriver.

  L'objectif de la partie 2 est de compter le nombre de tuiles se trouvant entourées par la zone des tuyaux.

  Exemple:
  OOOOOOOOOO
  OS------7O
  O|F----7|O
  O||OOOO||O
  O||OOOO||O
  O|L-7F-J|O
  O|II||II|O
  OL--JL--JO
  OOOOOOOOOO

  Les O représentent les tuiles se trouvant à l'extérieur, et les I sont entourées par les tuyaux.

  En remplaçant les caractères, on comprend mieux pourquoi les O au centre sont à l'extérieur du réseau:
  OOOOOOOOOO
  O╭------╮O
  O|╭----╮|O
  O||OOOO||O
  O||OOOO||O
  O|╰-╮╭-╯|O
  O|II||II|O
  O╰--╯╰--╯O
  OOOOOOOOOO

*/
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

// Structure représentant un point dans un espace bidimensionnel.
struct Point
{
    int x, y;

    // Surcharge de l'opérateur d'égalité pour comparer deux points.
    // Deux points sont égaux si leurs coordonnées x et y sont identiques.
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

// Définition d'une map associant un caractère à un vecteur de points.
map<char, vector<Point>> dir = {
    {'|', {{0, -1}, {0, 1}}},                 // Tuyau vertical reliant le nord et le sud.
    {'-', {{-1, 0}, {1, 0}}},                 // Tuyau horizontal reliant l'est et l'ouest.
    {'L', {{0, -1}, {1, 0}}},                 // Virage à 90 degrés reliant le nord et l'est.
    {'J', {{0, -1}, {-1, 0}}},                // Virage à 90 degrés reliant le nord et l'ouest.
    {'7', {{0, 1}, {-1, 0}}},                 // Virage à 90 degrés reliant le sud et l'ouest.
    {'F', {{0, 1}, {1, 0}}},                  // Virage à 90 degrés reliant le nord et l'est.
    {'S', {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}} // Position de départ, peut aller dans toutes les directions.
};

const char EMPTY_CELL = '.';   // Représentant une cellule vide dans la grille.
const char LOOP_CELL = 'x';    // Représentant une cellule dans le réseau de tuyaux.
const char IN_LOOP_CELL = 'I'; // Représentant une cellule qui n'appartient pas au reseau mais qui est entourée par lui.

vector<string> grid;      // Vecteur de chaînes de caractères représentant la grille principale.
vector<string> otherGrid; // Vecteur de chaînes de caractères représentant une autre grille utilisé pour le traitement.
vector<Point> poly;       // Vecteur de points représentant un polygone a partir du réseau de tuyaux.
Point start;              // Point représentant la position de départ.

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
        // Ajout de la ligne à la grille.
        grid.push_back(line);

        // Recherche de la position du point de départ 'S' dans la ligne actuelle.
        int a = line.find('S');

        // Si 'S' est trouvé dans la ligne, met à jour la position de départ.
        if (a != string::npos)
        {
            start = {a, grid.size() - 1};
        }
    }
    file.close();

    return 0;
}

// Fonction qui effectue le traitement de la Partie 1.
int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Récupération de la largeur et de la hauteur de la grille.
    int width = grid[0].size();
    int height = grid.size();

    // Initialisation d'une grille aux dimensions identiques à celle de la grille principale.
    for (int y = 0; y < height; y++)
    {
        otherGrid.push_back("");
        for (int x = 0; x < width; x++)
        {
            otherGrid[y].push_back(EMPTY_CELL);
        }
    }

    // Marquage du point de départ comme appartenant au réseau de tuyaux.
    otherGrid[start.y][start.x] = LOOP_CELL;

    // Compteur d'étapes parcourues.
    int count = 0;

    // Initialisation d'une pile pour le parcours en profondeur.
    stack<Point> q;
    q.push(start);

    // Parcours en profondeur.
    while (!q.empty())
    {
        // Récupération du point actuel de la pile.
        Point currentPoint = q.top();
        q.pop();
        count++;

        // Ajout du point au polygone.
        poly.push_back(currentPoint);

        // Exploration des voisins du point actuel en fonction du type de tuyau.
        for (Point d : dir[grid[currentPoint.y][currentPoint.x]])
        {
            // Calcul des coordonnées du voisin.
            Point neighboor = {currentPoint.x + d.x, currentPoint.y + d.y};

            // Vérification si les coordonnées du voisin sont valides.
            if (neighboor.x >= 0 && neighboor.x < width && neighboor.y >= 0 && neighboor.y < height)
            {
                // Vérification si le voisin est une cellule vide et n'a pas déjà été visité.
                if (otherGrid[neighboor.y][neighboor.x] == EMPTY_CELL && grid[neighboor.y][neighboor.x] != '.')
                {
                    // Vérification de la validité de la connexion avec le voisin.
                    bool valid = false;
                    for (Point dirPipe : dir[grid[neighboor.y][neighboor.x]])
                    {
                        Point _p = {neighboor.x + dirPipe.x, neighboor.y + dirPipe.y};
                        if (_p == currentPoint)
                        {
                            valid = true;
                            break;
                        }
                    }

                    // Si la connexion est valide, ajoute le voisin à la pile et le marque comme appartenant au réseau.
                    if (valid)
                    {
                        q.push(neighboor);
                        otherGrid[neighboor.y][neighboor.x] = LOOP_CELL;
                    }
                }
            }
        }
    }

    return floor(count / 2);
}

// Fonction qui vérifie si un point se trouve à l'intérieur d'un polygone.
bool inPolygon(Point p)
{
    // Initialise la variable indiquant si le point est à l'intérieur du polygone.
    bool inside = false;

    // Parcourt chaque segment du polygone.
    for (int i = 0; i < poly.size(); i++)
    {
        // Récupère les points extrémités du segment.
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % poly.size()];

        // Vérifie si le point p se trouve à la verticale du segment (p1, p2).
        bool isVertical = (p1.y > p.y) != (p2.y > p.y);
        if (isVertical)
        {
            // Vérifie si le point p se trouve à gauche de l'intersection avec le segment.
            bool isLeftOfIntersection = (p.x < (p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y) + p1.x);

            // Si le point p se trouve à la verticale du segment et à gauche de l'intersection,
            // change l'état de 'inside' (à l'intérieur ou à l'extérieur du polygone).
            if (isLeftOfIntersection)
            {
                inside = !inside;
            }
        }
    }

    return inside;
}

// Fonction qui effectue le traitement de la Partie 2.
int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Compteur pour le nombre de tuiles entourées par le réseau de tuyaux.
    int count = 0;

    // Parcours de chaque cellule dans la grille auxiliaire.
    for (int y = 0; y < otherGrid.size(); y++)
    {
        for (int x = 0; x < otherGrid[0].size(); x++)
        {
            // Vérifie si la cellule est vide.
            if (otherGrid[y][x] == EMPTY_CELL)
            {
                // Vérifie si la cellule est à l'intérieur du polygone formé par le réseau de tuyaux.
                if (inPolygon({x, y}))
                {
                    // Incrémente le compteur et marque la cellule comme entourée par le réseau.
                    count++;
                    otherGrid[y][x] = IN_LOOP_CELL;
                }
            }
        }
    }
    return count;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processP1();
    int part2 = processP2();

    for (int y = 0; y < otherGrid.size(); y++)
    {
        for (int x = 0; x < otherGrid[0].size(); x++)
        {
            cout << otherGrid[y][x];
        }
        cout << '\n';
    }

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}