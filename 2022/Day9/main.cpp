/*
L'entrée représente une liste de mouvements dans une direction avec le nombre de fois qu'il faut répéter ce mouvement.
Il faut ensuite effectuer les deplacement parlé précédament sur la "tête" d'une corde puis simmulé le déplacement de la queue en fonction.
Pour la partie 1, on simule avec une corde de longueur 2 (la tête et la queue). Il faut retourner le nombre de cases uniques visitées par la queue.
Pour la partie 2, on simule avec une corde de longueur 10. Il faut retourner le nombre de cases uniques visitées par la fin de la queue.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

struct point
{
  int x, y;
  set<string> visited;
};

point rope[10] = {
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}},
    {0, 0, {}}};

// Cette fonction prend en paramètre le point à mettre à jour (t) et le point précédent (h) pour calculer le déplacement de t.
void updateTail(point *h, point *t)
{
  int dx = h->x - t->x; // Différence sur l'axe vertical.
  int dy = h->y - t->y; // Différence sur l'axe horizontal.

  if (dx == 0 && dy == 0) //  Si les différences sont égales à 0, t et h sont sur la même case.
    ;
  else if (dx == 0 || dy == 0) // il y a une différence sur un seul axe,
  {
    if (abs(dx) == 1 | abs(dy) == 1) // mais cette différence n'est pas assez grande pour avoir besoin de déplacer la queue.
      ;
    else if (dx != 0) // Différence sur l'axe horizontal suffisante.
    {
      t->x += dx > 0 ? 1 : -1;
    }
    else // Différence sur l'axe vertical suffisante.
    {
      t->y += dy > 0 ? 1 : -1;
    }
  }
  // Il y a une différence sur les 2 axes, dont une est > 1.
  //(Si les 2 différences absolues sont égales à 1, on se trouve à la diagonale de la tête donc le mouvement n'est pas nécessaire).
  else if (abs(dx) > 1 || abs(dy) > 1)
  {
    if (dx > 0 && dy > 0) // Mouvement Bottom Right
    {
      t->x += 1;
      t->y += 1;
    }
    else if (dx > 0 && dy < 0) // Mouvement Top Right
    {
      t->x += 1;
      t->y -= 1;
    }
    else if (dx < 0 && dy < 0) // Mouvement Top Left
    {
      t->x -= 1;
      t->y -= 1;
    }
    else if (dx < 0 && dy > 0) // Mouvement Bottom Left
    {
      t->x -= 1;
      t->y += 1;
    }
  }
  t->visited.insert(to_string(t->x) + " " + to_string(t->y)); // Ajouter à la liste unique des visités.
}
// Fonction permettant d'exécuter la fonction updateTail sur tous les éléments de la corde.
void updateTails()
{
  for (int i = 1; i < 10; i++)
  {
    updateTail(&rope[i - 1], &rope[i]);
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
  rope[1].visited.insert("0 0");
  rope[9].visited.insert("0 0");
  while (getline(file, line))
  {
    int n = stoi(line.substr(2));
    // Effectuer le déplacement de la tête.
    for (int i = 0; i < n; i++)
    {
      if (line[0] == 'U')
      {
        rope[0].y--;
      }
      else if (line[0] == 'D')
      {
        rope[0].y++;
      }
      else if (line[0] == 'R')
      {
        rope[0].x++;
      }
      else
      {
        rope[0].x--;
      }
      updateTails();
    }
  }
  file.close();
  cout << "Partie1: " << rope[1].visited.size() << '\n';
  cout << "Partie2: " << rope[9].visited.size();

  return 0;
}