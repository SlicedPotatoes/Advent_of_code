/*
Dans l'input, on retrouve des groupes de nombres séparés par des lignes vides.
Pour la partie 1, le but est de trouver la somme du groupe ayant la valeur la plus élevée.
Pour la partie 2, le but est de trouver la somme des 3 groupes ayant les valeurs les plus élevées.
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int arr[3];

void addArray(int i)
{
  if (i > arr[0])
  {
    arr[0] = i;
  }
  if (arr[0] > arr[1])
  {
    int temp = arr[1];
    arr[1] = arr[0];
    arr[0] = temp;
  }
  if (arr[1] > arr[2])
  {
    int temp = arr[2];
    arr[2] = arr[1];
    arr[1] = temp;
  }
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
  int sum = 0;
  while (getline(file, line))
  {
    if (!line.empty())
    {
      sum += stoi(line);
    }
    else
    {
      addArray(sum);
      sum = 0;
    }
  }
  file.close();
  addArray(sum);
  cout << "Part1: " << arr[2] << '\n';
  cout << "Part2: " << arr[0] + arr[1] + arr[2];
  return 0;
}