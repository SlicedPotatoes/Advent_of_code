/*
L'input eprésente des commandes Linux qui parcourent une arborescence de fichiers (cd et ls) avec ce que retourne la commande.
Le but est de recréer l'arborescence des dossiers avec les fichiers qu'ils contiennent.
Pour la partie 1, il faut calculer la somme des tailles des dossiers ayant une taille inférieure à 100 000.
Pour la partie 2, il m'est précisé que l'espace total du disque est de 70 000 000 et que pour exécuter la mise à jour, j'ai besoin de 30 000 000 d'espace libre.
Je dois retourner la taille du dossier le plus petit à supprimer (1 seul) qui permettrait d'atteindre l'espace libre nécessaire à la mise à jour.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

const int MAX_SPACE = 70 * 1000 * 1000;
const int SPACE_NEEDED = 30 * 1000 * 1000;

vector<string> split(string s, char delim)
{
  vector<string> result;
  string element;
  istringstream tokenStream(s);

  while (getline(tokenStream, element, delim))
  {
    result.push_back(element);
  }

  return result;
}
// Structure pour un fichier
struct fileC
{
  string nom;
  int size;
};
// Structure pour un dossier
struct folder
{
  folder *parent;
  string name;
  map<string, folder *> childs;
  vector<fileC *> files;
  int size = 0;
};

vector<folder *> allNode;                     // Un tableau contenant toutes les nodes
folder *root = new folder{NULL, "/", {}, {}}; // Root de l'arbre
folder *currentNode = NULL;                   // Node actuel pendant sa création

// Fonction permettant de changer le noeud courant avec la commande cd
void setCurrentNode(string arg)
{
  if (arg == "/")
  {
    currentNode = root;
  }
  else if (arg == "..")
  {
    currentNode = currentNode->parent;
  }
  else
  {
    currentNode = currentNode->childs[arg];
  }
}

bool isNumber(string s)
{
  try
  {
    stoi(s);
    return true;
  }
  catch (exception e)
  {
    return false;
  }
}
// Fonction de débogage pour afficher l'arbre grâce à l'algorithme DFS
void depthFirstSearch(folder *f, int depth)
{
  for (int i = 0; i < depth; i++)
  {
    cout << ' ';
  }
  cout << f->name << ' ' << f->size << '\n';
  for (int i = 0; i < (int)f->files.size(); i++)
  {
    fileC *fc = f->files[i];
    for (int j = 0; j < depth + 1; j++)
    {
      cout << ' ';
    }
    cout << '(' << fc->nom << ' ' << fc->size << ")\n";
  }
  for (auto it = f->childs.begin(); it != f->childs.end(); it++)
  {
    depthFirstSearch(it->second, depth + 1);
  }
}
// Calculer la taille pour chaque dossier avec DFS
void computeSize(folder *node)
{
  int sum = 0;
  int length = node->files.size();
  for (int i = 0; i < length; i++)
  {
    sum += node->files[i]->size;
  }
  for (auto i = node->childs.begin(); i != node->childs.end(); i++)
  {
    folder *child = i->second;
    computeSize(child);
    sum += child->size;
  }
  node->size = sum;
}

int main()
{
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
    vector<string> splitedLine = split(line, ' ');
    if (splitedLine[0] == "$" && splitedLine[1] == "cd")
    {
      setCurrentNode(splitedLine[2]);
    }
    else if (splitedLine[0] == "dir")
    {
      folder *node = new folder{currentNode, splitedLine[1], {}, {}};
      currentNode->childs[splitedLine[1]] = node;
      allNode.push_back(node);
    }
    else if (isNumber(splitedLine[0]))
    {
      fileC *f = new fileC{splitedLine[1], stoi(splitedLine[0])};
      currentNode->files.push_back(f);
    }
  }
  file.close();

  computeSize(root);

  // Part 1
  int sumPart1 = 0;
  for (folder *node : allNode)
  {
    if (node->size < 100000)
    {
      sumPart1 += node->size;
    }
  }
  // Part 2
  int spaceObjectif = MAX_SPACE - SPACE_NEEDED;
  int lessDiff = MAX_SPACE;
  int sizeFolderDelete = MAX_SPACE;
  int sizeRoot = root->size;
  for (folder *node : allNode)
  {
    int sizeAfterDelete = sizeRoot - node->size;
    int diff = spaceObjectif - sizeAfterDelete;
    if (diff > 0 && abs(diff) < lessDiff)
    {
      lessDiff = abs(diff);
      sizeFolderDelete = node->size;
    }
  }

  cout << "Part1: " << sumPart1 << '\n';
  cout << "Part2: " << sizeFolderDelete;
  return 0;
}