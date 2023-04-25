/*
L'input représente une forêt sous forme de grille où chaque élément est la hauteur d'un arbre.
Dans la partie 1, il faut compter le nombre d'arbres visibles depuis l'extérieur de la grille.
Un arbre est visible s'il est plus grand que les arbres plus proches du bord que lui, dans sa direction.
Dans la partie 2, il faut calculer un score pour chaque arbre.
Pour ce faire, on doit compter le nombre d'arbres visibles depuis celui-ci dans les 4 directions. Le score est le produit du nombre d'arbres visibles pour chaque direction.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int LENGTH_MAP = 99;
int map[LENGTH_MAP][LENGTH_MAP];

// Fonction utilisée dans la partie 1 pour trouver tous les arbres visibles depuis l'extérieur
int visibleTree()
{
  // Initialise array
  int array[LENGTH_MAP][LENGTH_MAP] = {{0, 0}};
  // Tous les arbres sur le bord de la carte sont visibles depuis l'extérieur
  for (int i = 0; i < LENGTH_MAP; i++)
  {
    array[i][0] = 1;
    array[0][i] = 1;
    array[i][LENGTH_MAP - 1] = 1;
    array[LENGTH_MAP - 1][i] = 1;
  }

  // On parcourt notre grille. "i" peut représenter une ligne ou une colonne, de même pour "j"
  for (int i = 0; i < LENGTH_MAP; i++)
  {
    int maxHeightUp = 0;
    int maxHeightDown = 0;
    int maxHeightLeft = 0;
    int maxHeightRight = 0;

    for (int j = 0; j < LENGTH_MAP; j++)
    {
      if (map[i][j] > maxHeightRight) // Cette condition parcourt le tableau de manière "classique"
      {
        array[i][j] = 1;
        maxHeightRight = map[i][j];
      }
      if (map[i][LENGTH_MAP - 1 - j] > maxHeightLeft) // Cette condition parcourt le tableau ligne par ligne mais en commençant depuis la fin de la ligne
      {
        array[i][LENGTH_MAP - 1 - j] = 1;
        maxHeightLeft = map[i][LENGTH_MAP - 1 - j];
      }
      if (map[j][i] > maxHeightDown) // Cette condition parcourt le tableau colonne par colonne
      {
        array[j][i] = 1;
        maxHeightDown = map[j][i];
      }
      if (map[LENGTH_MAP - 1 - j][i] > maxHeightUp) // Cette condition parcourt le tableau colonne par colonne mais en commençant depuis la fin de la colonne
      {
        array[LENGTH_MAP - 1 - j][i] = 1;
        maxHeightUp = map[LENGTH_MAP - 1 - j][i];
      }
    }
  }
  int total = 0;
  // On compte le total d'arbres visibles
  for (int i = 0; i < LENGTH_MAP; i++)
  {
    for (int j = 0; j < LENGTH_MAP; j++)
    {
      if (array[i][j] == 1)
      {
        total++;
      }
    }
  }
  return total;
}

/*
Fonction utilisée dans la partie 2 qui parcourt toutes les cases de l'array,
compte les arbres visibles et calcule le score de chaque arbre pour finalement retourner le plus élevé.
*/
int maxScoreTree()
{
  int maxScore = 0;
  for (int i = 0; i < LENGTH_MAP; i++)
  {
    for (int j = 0; j < LENGTH_MAP; j++)
    {
      int treeHeight = map[i][j];
      int count[4] = {0};
      for (int k = i - 1; k >= 0; k--) // UP
      {
        count[0]++;
        if (treeHeight <= map[k][j])
        {
          break;
        }
      }
      for (int k = i + 1; k < LENGTH_MAP; k++) // Down
      {
        count[1]++;
        if (treeHeight <= map[k][j])
        {
          break;
        }
      }
      for (int k = j - 1; k >= 0; k--) // Left
      {
        count[2]++;
        if (treeHeight <= map[i][k])
        {
          break;
        }
      }
      for (int k = j + 1; k < LENGTH_MAP; k++) // Right
      {
        count[3]++;
        if (treeHeight <= map[i][k])
        {
          break;
        }
      }
      int score = count[0] * count[1] * count[2] * count[3];
      if (score > maxScore)
      {
        maxScore = score;
      }
    }
  }
  return maxScore;
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
  int index = 0;
  while (getline(file, line))
  {
    int length = line.length();
    for (int i = 0; i < length; i++)
    {
      string s(1, line[i]);
      map[index][i] = stoi(s);
    }
    index++;
  }
  file.close();
  int partie1 = visibleTree();
  int partie2 = maxScoreTree();
  cout << "Partie1: " << partie1 << '\n';
  cout << "Partie2: " << partie2;
  return 0;
}