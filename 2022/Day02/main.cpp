/*
Dans l'input, sur chaque ligne, on retrouve des lettres A à C, puis X à Z séparées par un espace
Il faut simuler le jeu pierre papier ciseaux pour calculer un score en respectant ces règles:
- Si on gagne : +6 points
- Si match nul : +3 points
- Si on perd : 0 point
- Si on joue "Pierre" : +1 point
- Si on joue "Papier" : +2 points
- Si on joue "Ciseaux" : +3 points
A représente "Pierre", B représente "Papier" et C représente "Ciseaux".

Dans la partie 1 : X représente "Pierre", Y représente "Papier" et Z représente "Ciseaux".
Dans la partie 2 : X représente qu'il faut que je perde la manche, Y représente un match nul et Z représente une victoire.

Pour chacune des parties, je dois retourner le score que j'ai obtenu.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

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

  int scorePart1 = 0;
  int scorePart2 = 0;
  int win[3] = {2, 0, 1};  // On passe le coup de "Joueur1" Si "Joueur2" a la valeurs qui ressort de ce tableau alors "Joueur1" gagne la manche
  int lose[3] = {1, 2, 0}; // L'inverse du tableau win
  while (getline(file, line))
  {
    int enemy = line[0] - 'A'; // C'est 2 lignes permettent d'avoir a partir de la valeur de l'input des valeurs entre 0 et 2
    int me = line[2] - 'X';    // A = 0, B = 1, C = 2 et parreil pour X, Y et Z
    // Partie 1
    if (enemy == win[me])
    {
      scorePart1 += 6;
    }
    else if (enemy == me)
    {
      scorePart1 += 3;
    }
    scorePart1 += me + 1;
    // Partie 2
    if (me == 1) // Je dois faire un match nul
    {
      scorePart2 += 3;
      me = enemy;
    }
    else if (me == 2) // Je dois gagné
    {
      scorePart2 += 6;
      me = lose[enemy];
    }
    else // Je dois perdre
    {
      me = win[enemy];
    }
    scorePart2 += me + 1;
  }
  file.close();
  cout << "Part1: " << scorePart1 << '\n';
  cout << "Part2: " << scorePart2;
  return 0;
}