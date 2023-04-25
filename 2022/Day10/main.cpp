/*
L'input de ce challenge représente des instructions processeur à interpréter :
- noop: rien ne se passe mais le cycle d'horloge est incrémenté
- addX: ajouter une valeur à la variable X, l'instruction prend 2 cycles et la valeur est modifiée pendant le 2ème cycle
Pour la partie 1, il faut pour le cycle 20 et tous les 40 cycles après faire la somme de (num cycle * X) et la retourner.
Pour la partie 2, il faut utiliser la méthode "Racing the Beam". La variable X représente les coordonnées au centre d'un sprite de 3 de longueur.
Les cycles 1 à 40 affichent la première ligne de l'image, etc. jusqu'à 6 lignes.
À chaque début de cycle, si le sprite se trouve à l'endroit qu'on est en train de calculer le pixel, le pixel sera blanc (#), sinon noir (.).
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

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

const int SCREEN_WIDTH = 40;
const int SCREEN_HEIGHT = 6;
char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

int X = 1;
int cycle = 0;
int sumPart1 = 0;
// Fonction qui effectue des actions à chaque cycle, elle gère l'affichage, l'incrémentation du cycle et permet de calculer la somme pour la partie 1
void changeCycle()
{
  int screenLine = (cycle - (cycle % 40)) / 40; // La ligne où le pixel sera calculé à ce cycle
  int screenCol = cycle % 40;                   // La colonne où le pixel sera calculé à ce cycle
  if (screenLine < 6)
  {
    bool onScreen = (X - 1 == screenCol) || (X == screenCol) || (X + 1 == screenCol); // Vérifie si le sprite se trouve à l'endroit où l'on compute le pixel actuel
    screen[screenLine][screenCol] = onScreen ? '#' : '.';                             // Affiche un pixel noir ou blanc en fonction
  }
  cycle++;
  if (cycle == 20 || (cycle - 20) % 40 == 0) // Partie 1 : calcule cycle * X et l'ajoute à la somme
  {
    sumPart1 += cycle * X;
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
  changeCycle();
  while (getline(file, line))
  {
    vector<string> instruction = split(line, ' ');
    changeCycle();                // Incrémente le cycle
    if (instruction[0] == "addx") // Si l'instruction est addX, change la valeur de X et incrémente le cycle
    {
      X += stoi(instruction[1]);
      changeCycle();
    }
  }
  file.close();
  cout << "Part1: " << sumPart1 << '\n';
  cout << "Part2: \n";
  for (int i = 0; i < SCREEN_HEIGHT; i++)
  {
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
      cout << screen[i][j];
    }
    cout << '\n';
  }
  return 0;
}