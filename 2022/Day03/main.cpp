/*
Chaque ligne représente le contenu d'un sac, chaque caractère représente un type d'objet différent (a != A).
Pour chaque ligne, il y a un nombre pair d'éléments. La première moitié représente la poche 1 et la deuxième moitié représente la poche 2.
Pour la partie 1: je dois retrouver l'objet du même type qui se trouve dans les 2 poches et retourner la somme de ses objets de chaque ligne. (a = 1, z = 26, A = 27, Z = 52).
Pour la partie 2: je dois retrouver, pour chaque groupe de 3 lignes, un objet en commun entre ces 3 lignes, et retourner la somme totale de ces objets pour chaque groupe."
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

const int CHAR_REF_LOWER = 'a';
const int CHAR_REF_UPPER = 'A';

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
  int sumPart1 = 0;
  int object[52] = {0};
  int sumPart2 = 0;
  int index = 0; // permet de séparé les lignes en groupes

  while (getline(file, line))
  {
    // Part 1
    int length = line.length() / 2;
    string poche1 = line.substr(0, length);
    string poche2 = line.substr(length, length * 2);
    for (int i = 0; i < length; i++) // On parcours la premiere moitier de la ligne et cherche pour chaque charactere si il se trouve dans la 2eme moitier
    {
      char c = poche1[i];
      if (poche2.find(c) != -1)
      {
        if (islower(c))
        {
          sumPart1 += c - CHAR_REF_LOWER + 1;
        }
        else
        {
          sumPart1 += c - CHAR_REF_UPPER + 27;
        }
        break;
      }
    }
    // Part 2
    bool registre[52] = {false};
    for (int i = 0; i < length * 2; i++) // Pour chaque charactere incrémenté un compteur si il n'a pas deja été vue dans cette ligne
    {
      int c;
      if (islower(line[i]))
      {
        c = line[i] - CHAR_REF_LOWER;
      }
      else
      {
        c = line[i] - CHAR_REF_UPPER + 26;
      }
      if (!registre[c])
      {
        object[c]++;
        registre[c] = true;
      }
    }
    index++;
    if (index == 3) // Si on a un groupe de 3, cherché un charactere qui a été vue 3 fois et renitilisé l'index et les compteurs pour la suite
    {
      index = 0;
      for (int i = 0; i < 26; i++)
      {
        int cLower = i;
        int cUpper = i + 26;
        if (object[cLower] == 3)
        {
          sumPart2 += cLower + 1;
        }
        else if (object[cUpper] == 3)
        {
          sumPart2 += cUpper + 1;
        }
        object[cLower] = 0;
        object[cUpper] = 0;
      }
    }
  }
  file.close();
  cout << "Part1: " << sumPart1 << '\n';
  cout << "Part2: " << sumPart2;
  return 0;
}