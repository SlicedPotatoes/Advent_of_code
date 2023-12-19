/*
  L'entrée représente une grille composée de miroirs et de séparateurs:
  - "." représente une case vide.
  - "|" représente un séparateur vertical; si un faisceau arrive de l'Ouest/Est, sa trajectoire est divisée en deux vers
  le Nord/Sud.
  - "-" représente un séparateur horizontal; si un faisceau arrive du Nord/Sud, sa trajectoire est divisée en deux vers
  l'Ouest/Est.
  - "/" et "\" représentent des miroirs qui redirigent le faisceau en fonction de leur provenance.

  Partie 1: L'objectif est de déterminer le nombre de cases traversées par le faisceau lumineux, en commençant en 0,0 et
  en se déplaçant vers la droite.

  Partie 2: L'objectif est de trouver le point de départ du faisceau qui couvre le plus grand nombre de cases de la
  grille, puis de retourner le nombre ainsi obtenu.
*/
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

vector<string> grid;

// Représente un point sur un plan 2D
struct Point
{
    int x;
    int y;

    struct Hash
    {
        size_t operator()(const Point &obj) const
        {
            hash<int> intHasher;
            return intHasher(obj.x) ^ intHasher(obj.y);
        }
    };

    // Opérateur de comparaison pour déterminer l'égalité entre deux points
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

// Représente un noeud avec une position et une direction
struct Node
{
    Point pos;
    Point dir;
};

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
        grid.push_back(line);
    }
    file.close();
    return 0;
}

// Fonction de hachage pour la structure Node
size_t hashNode(Node &n)
{
    return n.dir.x * 256 * 256 * 256 + n.dir.y * 256 * 256 + n.pos.x * 256 + n.pos.y;
}

// Fonction de parcours en largeur (BFS) pour déterminer le nombre de cases traversées par un faisceau lumineux
int bfs(Node n)
{
    // Ensemble pour stocker les points visités et les noeuds uniques
    unordered_set<Point, Point::Hash> visitedPoint;
    unordered_set<size_t> uniqueNode;

    // File pour le parcours en largeur
    queue<Node> q;
    q.push(n);

    while (q.size() != 0)
    {
        Node currentNode = q.front();
        q.pop();

        // Calcul de la clé de hachage pour le nœud actuel
        size_t key = hashNode(currentNode);

        // Vérifie si le noeud a déjà été traité
        if (uniqueNode.find(key) != uniqueNode.end())
        {
            continue;
        }

        // Ajoute la position du noeud actuel à l'ensemble des points visités
        visitedPoint.insert(currentNode.pos);
        // Ajoute la clé de hachage à l'ensemble des noeuds uniques
        uniqueNode.insert(key);

        // Calcule la position suivante en fonction de la direction actuelle
        Point nextPos = {currentNode.pos.x + currentNode.dir.x, currentNode.pos.y + currentNode.dir.y};

        // Vérifie si la position suivante est valide dans la grille
        if (nextPos.x >= 0 && nextPos.x < grid[0].size() && nextPos.y >= 0 && nextPos.y < grid.size())
        {
            char nextCase = grid[nextPos.y][nextPos.x];

            // Traite les différents types de cases dans la grille
            if (nextCase == '.')
            {
                q.push({nextPos, currentNode.dir});
            }
            else if (nextCase == '|')
            {
                if (currentNode.dir.x != 0)
                {
                    q.push({nextPos, {0, 1}});
                    q.push({nextPos, {0, -1}});
                }
                else
                {
                    q.push({nextPos, currentNode.dir});
                }
            }
            else if (nextCase == '-')
            {
                if (currentNode.dir.y != 0)
                {
                    q.push({nextPos, {1, 0}});
                    q.push({nextPos, {-1, 0}});
                }
                else
                {
                    q.push({nextPos, currentNode.dir});
                }
            }
            else if (nextCase == '/')
            {
                // Gère la redirection du faisceau en fonction du miroir '/'
                if (currentNode.dir.y == -1)
                {
                    q.push({nextPos, {1, 0}});
                }
                else if (currentNode.dir.y == 1)
                {
                    q.push({nextPos, {-1, 0}});
                }
                else if (currentNode.dir.x == -1)
                {
                    q.push({nextPos, {0, 1}});
                }
                else
                {
                    q.push({nextPos, 0, -1});
                }
            }
            else if (nextCase == '\\')
            {
                // Gère la redirection du faisceau en fonction du miroir '\'
                if (currentNode.dir.y == -1)
                {
                    q.push({nextPos, {-1, 0}});
                }
                else if (currentNode.dir.y == 1)
                {
                    q.push({nextPos, {1, 0}});
                }
                else if (currentNode.dir.x == -1)
                {
                    q.push({nextPos, {0, -1}});
                }
                else
                {
                    q.push({nextPos, {0, 1}});
                }
            }
        }
    }

    // Retourne le nombre de points visités, soustrait 1 pour exclure le point de départ
    return visitedPoint.size() - 1;
}

// Fonction pour traiter la Partie 1 du problème
int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Appelle la fonction BFS avec le point de départ (-1, 0) et la direction (1, 0)
    return bfs({{-1, 0}, {1, 0}});
}

// Fonction pour traiter la Partie 2 du problème
int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int maxE = 0;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int e = 0;
            if (j == 0)
            {
                e = bfs({{-1, i}, {1, 0}});
            }
            else
            {
                e = bfs({{int(grid[0].size()), i}, {-1, 0}});
            }
            if (e > maxE)
            {
                maxE = e;
            }
        }
    }
    for (int i = 0; i < grid[0].size(); i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int e = 0;
            if (j == 0)
            {
                e = bfs({{i, -1}, {0, 1}});
            }
            else
            {
                e = bfs({{i, int(grid.size())}, {0, -1}});
            }
            if (e > maxE)
            {
                maxE = e;
            }
        }
    }

    return maxE;
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