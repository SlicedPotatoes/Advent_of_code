/*
L'input représente une carte topographique avec des altitudes. Chaque point sur la carte est caractérisé par une lettre de 'a' (le point le plus bas) à 'z' (le point le plus élevé).
Dans la première partie, l'objectif est de déterminer le chemin le plus court pour se rendre du point "S" (qui a une altitude de 'a') au point "E" (qui a une altitude de 'z')
en utilisant le moins d'étapes possible. Les déplacements sur la carte sont soumis à des contraintes d'altitude spécifiques :
on ne peut se déplacer que d'une unité d'altitude vers le haut, par exemple en passant d'une case 'a' à une case 'b'.
Cependant, il est également permis de descendre de manière continue, par exemple en passant d'une case 'z' à une case 'a'.
La réponse attendue est le nombre d'étapes minimum nécessaires pour atteindre cet objectif.

Dans la partie 2, l'objectif est de déterminer quel point de départ "S" (les cases ayant une altitude de 'a') permettra d'obtenir le chemin le plus court pour atteindre l'objectif.
Une fois le point de départ trouvé, l'algorithme renverra le nombre d'étapes minimum nécessaire pour ce point trouvé.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>

using namespace std;

const int MAP_WIDTH = 101;
const int MAP_HEIGHT = 41;

const int INDEX_HELPER[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

struct Point
{
  int y;
  int x;
};

class Node
{
public:
  int id;        // Id unique du noeud, permettant de determiner s'il a été visité ou non
  float coutG;   // Le coût pour aller du point de départ à ce noeud
  int elevation; // Valeur numérique de l'élévation
  Point coord;   // Coordonnées sur la map
  vector<Node *> getChild(Node grid[MAP_HEIGHT][MAP_WIDTH]) const
  {
    // Récupère les childs de la node
    vector<Node *> v;
    for (int i = 0; i < 4; i++)
    {
      const int x = coord.x + INDEX_HELPER[i][0];
      const int y = coord.y + INDEX_HELPER[i][1];
      if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
      {
        v.push_back(&grid[y][x]);
      }
    }
    return v;
  }
};

Node graph[MAP_HEIGHT][MAP_WIDTH];
Node *startNode;
Node *endNode;
vector<Node *> startP2;

// Fonction de recherche de chemin par BFS
int solverBFS(Node *start)
{
  start->coutG = 0;
  queue<Node *> queue;        // Queue pour stocker les noeuds a visiter
  unordered_set<int> visited; // Liste des noeuds deja visité
  queue.push(start);          // Ajout du noeud de départ a la queue
  while (!queue.empty())
  {
    const Node *currentNode = queue.front(); // Récupération du premier element de la queue
    queue.pop();                             // Suppression du premiere element de la queue

    // Si le noeud a deja été visité, on passe au suivant
    if (visited.find(currentNode->id) != visited.end())
    {
      continue;
    }

    // Si le noeud est le noeud de fin, on quitte la boucle
    if (currentNode == endNode)
    {
      break;
    }
    // On insert le noeud dans la liste visited
    visited.insert(currentNode->id);
    // On récupère les childs du noeud
    vector<Node *> childs = currentNode->getChild(graph);
    for (int i = 0; i < childs.size(); i++)
    {
      Node *child = childs[i];
      int diffElevation = child->elevation - currentNode->elevation; // On calcule la différence d'élévation entre le noeud et l'enfant
      // Si cette différence est <= à 1 (permet de passer à un dénivelé de 1 de hauteur en plus ou de descendre en continu)
      if (diffElevation <= 1)
      {
        if (child->coutG > currentNode->coutG + 1)
        {
          child->coutG = currentNode->coutG + 1;
        }
        if (visited.find(child->id) == visited.end())
        {
          queue.push(child);
        }
      }
    }
  }
  return endNode->coutG;
}

int main()
{
  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day12\\input.txt";
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
    return 1;
  }
  string line;
  int index = 0;
  // Parcourir l'input pour remplir le graph
  while (getline(file, line))
  {
    for (int i = 0; i < MAP_WIDTH; i++)
    {
      char c = line[i];
      graph[index][i] = Node{index * MAP_WIDTH + i, numeric_limits<float>::max(), c - 'a', {index, i}};
      // Initialiser startNode
      if (c == 'S')
      {
        startNode = &graph[index][i];
        startNode->coutG = 0;
        startNode->elevation = 0;
      }
      // Initialiser endNode
      if (c == 'E')
      {
        endNode = &graph[index][i];
        endNode->elevation = 25;
      }
      // Dénivelé 'a' trouvé, à mettre dans startP2
      if (c == 'a')
      {
        startP2.push_back(&graph[index][i]);
      }
    }
    index++;
  }
  file.close();
  int part1 = solverBFS(startNode);
  int part2 = part1;
  for (int i = 0; i < startP2.size(); i++)
  {
    int value = solverBFS(startP2[i]);
    if (value < part2)
    {
      part2 = value;
    }
  }
  cout << "Part1: " << part1 << endl;
  cout << "Part2: " << part2 << endl;
  return 0;
}