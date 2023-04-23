/*
L'input contient une chaîne de caractères composée de lettres minuscules de a à z.
Dans la partie 1, je dois trouver la séquence de 4 caractères qui comporte des caractères uniques et afficher l'index de fin de cette séquence.
Dans la partie 2, je dois trouver la séquence de 14 caractères qui comporte des caractères uniques et afficher l'index de fin de cette séquence.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int CHAR_REF = 'a';
const int LENGTH_SEQUENCE_P1 = 4;
const int LENGTH_SEQUENCE_P2 = 14;
const int NB_LETTRES = 26;

/*
// Je pense que cette fonction pourrait être optimisée en parcourant la chaîne une seule fois.
int findUniqueSequence(string s, int n)
{
  int length = s.length();
  for (int i = 0; i < length; i++) // Pour chaque caractère de la séquence,
  {
    int dejavu[NB_LETTRES] = {false}; // on initialise un tableau de booléens permettant de voir si on a déjà vu le caractère,
    bool find = true;
    for (int j = 0; j < n; j++) // et on vérifie les n caractères suivants.
    {
      int c = s[i + j] - CHAR_REF;
      if (dejavu[c]) // Si le caractère est déjà vu, on passe au caractère suivant.
      {
        find = false;
        break;
      }
      dejavu[c] = true;
    }
    if (find) // On a trouvé une séquence unique de longueur n.
    {
      return i + n;
    }
  }
  return -1;
}
*/
// Après avoir remarqué que je pouvais optimiser la fonction, j'ai essayé d'implémenter l'idée que j'avais eue, et cela a donné cette nouvelle fonction plus rapide.
int findUniqueSequence(string s, int n)
{
  int length = s.length();
  bool dejavu[NB_LETTRES] = {false}; // On initialise un tableau de booléens permettant de voir si on a déjà vu le caractère,
  int sequence[LENGTH_SEQUENCE_P2];  // On initialise une file qui permettra de stocker la séquence pendant la recherche
  int tailleSequence = 0;            // Taille actuelle de la séquence
  int debutSequence = 0;             // Index de debut de la file
  int finSequence = 0;               // Index de fin de la file
  for (int i = 0; i < length; i++)   // On parcourt la chaîne de caractères une seule fois
  {
    int c = s[i] - CHAR_REF;
    if (dejavu[c]) // Si le caractère a déjà été vu dans la séquence
    {
      while (true) // Supprimez tous les caractères de la file jusqu'au caractère déjà vu.
      {
        int element = sequence[debutSequence];
        debutSequence++;
        tailleSequence--;
        dejavu[element] = false;
        if (debutSequence == LENGTH_SEQUENCE_P2)
          debutSequence = 0;
        if (element == c)
          break;
      }
    }
    sequence[finSequence] = c; // Mettre le caractère actuel dans la file
    finSequence++;
    tailleSequence++;
    dejavu[c] = true; // Mettre le caractère actuel comme déjà vu
    if (finSequence == LENGTH_SEQUENCE_P2)
      finSequence = 0;
    if (tailleSequence == n) // On a trouvé une séquence unique de longueur n
      return i + 1;
  }
  return -1;
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
  getline(file, line);
  int length = line.length();
  // Partie 1
  int partie1 = findUniqueSequence(line, LENGTH_SEQUENCE_P1);
  // Partie 2
  int partie2 = findUniqueSequence(line, LENGTH_SEQUENCE_P2);
  file.close();
  cout << "Partie 1: " << partie1 << '\n';
  cout << "Partie 2: " << partie2 << '\n';
  return 0;
}