/*
L'input est séparé en 2 parties par une ligne vide.
La première partie de l'input représente l'état d'une liste de piles.
La 2ème partie de l'input représente les mouvements à faire entre ces piles.
Pour les 2 parties de l'exercice, le programme doit retourner la concaténation de tous les éléments se trouvant en haut des piles.
Pour la partie 1, si je dois déplacer 3 éléments de la pile 1 à la pile 2, pour chaque élément, je déplace le premier (l'ordre final sur la pile 2 change).
Pour la partie 2, si je dois déplacer 3 éléments de la pile 1 à la pile 2, je dois déplacer les 3 premiers éléments en conservant leur ordre.
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

// Peut-être faut-il modifier les constantes selon l'input, car dans l'énoncé il n'y a pas de contraintes. J'ai mis ce qui correspondait à mon input

const int NB_LINE_INPUT = 9;
const int NB_PILES = 10;
const int MAX_ELEMENT = 56;

string inputPiles[NB_LINE_INPUT]; // Stockez les inputs qui forment la pile initiale pour les traiter plus tard
int actualIndexInputPile = 0;     // L'index actuel pour ajouter les inputs à l'array inputPiles

char pilesPart1[NB_PILES][MAX_ELEMENT]; // Le tableau de pile qui sera utilisé pour la partie 1 de l'exercice
int indexPilesPart1[NB_PILES] = {0};    // L'indice en haut de chaque pile dans le tableau pour la partie 1

char pilesPart2[NB_PILES][MAX_ELEMENT]; // L'équivalent pour la partie 2
int indexPilesPart2[NB_PILES] = {0};

int state = 0; // 0: initialisation | 1: traitement

void parseInputPiles() // Fonction qui crée des piles en fonction des entrées
{
  for (int i = actualIndexInputPile - 1; i > -1; i--) // Parcourt les inputs dans l'ordre inverse dans lequel ils ont été fournis
  {
    string input = inputPiles[i];
    int inputLength = input.length();
    for (int j = 0; j < inputLength; j += 4) // Séparez la chaîne en groupes de 4 caractères (un élément de la pile est représenté par [X] en comptant l'espace, ce qui donne un groupe de 4)
    {
      char c = input[j + 1];
      // J'ai ajouté cette condition car à la fin de la liste des piles, il y a une ligne qui représente les index de la pile que je ne veux pas utiliser
      // Cette condition permet également d'éviter d'ajouter des espaces lorsque cette partie de la pile est vide
      if (isalpha(c))
      {
        int idPile = (j / 4) + 1;
        int index = indexPilesPart1[idPile];
        pilesPart1[idPile][index] = c;
        pilesPart2[idPile][index] = c;
        indexPilesPart1[idPile]++;
        indexPilesPart2[idPile]++;
      }
    }
  }
}
// Fonction permettant de déplacer le premier élément d'une pile vers le haut d'une autre pile. Cette opération est répétée n fois
void moveElementPilePart1(int idPileFrom, int idPileTo, int n)
{
  int indexPileFrom = indexPilesPart1[idPileFrom];
  int indexPileTo = indexPilesPart1[idPileTo];
  for (int i = 0; i < n; i++)
  {
    pilesPart1[idPileTo][indexPileTo] = pilesPart1[idPileFrom][indexPileFrom - 1];
    indexPileFrom--;
    indexPileTo++;
  }
  indexPilesPart1[idPileFrom] = indexPileFrom;
  indexPilesPart1[idPileTo] = indexPileTo;
}
// Fonction permettant de déplacer les n premiers éléments d'une pile vers le haut d'une autre pile tout en conservant l'ordre original des éléments.
void moveElementPilePart2(int idPileFrom, int idPileTo, int n)
{
  int indexPileFrom = indexPilesPart2[idPileFrom];
  int indexPileTo = indexPilesPart2[idPileTo];
  for (int i = n; i != 0; i--)
  {
    pilesPart2[idPileTo][indexPileTo] = pilesPart2[idPileFrom][indexPileFrom - i];
    indexPileTo++;
  }
  indexPilesPart2[idPileFrom] -= n;
  indexPilesPart2[idPileTo] += n;
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
    if (state == 0)
    {
      if (!line.empty())
      {
        inputPiles[actualIndexInputPile] = line;
        actualIndexInputPile++;
      }
      else
      {
        parseInputPiles();
        state = 1;
      }
    }
    else
    {
      vector<string> operation = split(line, ' ');
      int idFrom = stoi(operation[3]);
      int idTo = stoi(operation[5]);
      int n = stoi(operation[1]);
      moveElementPilePart1(idFrom, idTo, n);
      moveElementPilePart2(idFrom, idTo, n);
    }
  }
  file.close();
  cout << "Part1: ";
  for (int i = 1; i < NB_PILES; i++)
  {
    int index = indexPilesPart1[i];
    cout << pilesPart1[i][index - 1];
  }
  cout << "\nPart2: ";
  for (int i = 1; i < NB_PILES; i++)
  {
    int index = indexPilesPart2[i];
    cout << pilesPart2[i][index - 1];
  }
  return 0;
}