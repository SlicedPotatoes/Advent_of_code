/*
  L'entrée du puzzle représente les murs d'une carte qui se lit comme suit :
  X1, Y1 => X2, Y2 => X3, Y3 => etc.
  Le mur commence à X1, Y1 et va jusqu'à X2, Y2, puis reprend de X2, Y2 jusqu'à X3, Y3, etc.
  Les murs ne peuvent être des diagonales.
  Il y a un "trou" par lequel du sable passe en X: 500 Y: 0.
  Il faut simuler le déplacement du sable selon les conditions suivantes :
  - Une unité de sable tombe quand l'unité précédente a "trouvé sa place".
  - Le sable tombe (augmente en Y) jusqu'à trouver un obstacle (mur ou une autre unité de sable).
  - S'il y a de la place à gauche, il continue de descendre sur la gauche.
  - S'il y a de la place à droite, il continue de descendre sur la droite.
  - Sinon, il a trouvé sa place.

  Dans la partie 1, le but est de retourner le nombre d'unités de sable nécessaires pour qu'il déborde de la plateforme la plus basse (le Y le plus élevé).
  Dans la partie 2, on ajoute une plateforme en dessous de la plateforme la plus basse, à une distance de 2 unités de celle-ci, et on continue la simulation jusqu'à ce que le trou par lequel le sable passe soit bouché. Ensuite, on retourne le nombre d'unités de sable nécessaires.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int MAP_SIZE = 1000;

char map[MAP_SIZE][MAP_SIZE];

int minX = INT_MAX;
int maxX = 0;
int minY = INT_MAX;
int maxY = 0;

// Fonction qui split un string en vector<string> en utilisant un délimiteur
vector<string> splitString(const string &input, const string &delimiter)
{
  vector<string> tokens;
  size_t start = 0, end = 0;

  while ((end = input.find(delimiter, start)) != string::npos)
  {
    if (end != start)
    {
      tokens.push_back(input.substr(start, end - start));
    }
    start = end + delimiter.length();
  }

  if (start < input.length())
  {
    tokens.push_back(input.substr(start));
  }

  return tokens;
}

// Récupère les coordonnées X Y d'un string au format X,Y
int *getCoord(string s)
{
  int *coord = new int[2];
  int i = s.find(',');
  coord[0] = stoi(s.substr(0, i));
  coord[1] = stoi(s.substr(i + 1));
  return coord;
}

// Affiche la map
void showMap(string s)
{
  cout << s << "\n";
  for (int y = minY; y <= maxY; y++)
  {
    for (int x = minX; x <= maxX; x++)
    {
      cout << map[y][x];
    }
    cout << "\n";
  }
}

// Ajoute un mur a la map en fonction des coordonnées de depart / de fin
void addLine(int x1, int y1, int x2, int y2)
{
  // Initialise les variables pour la boucle qui ajoute le mur en fonction de s'il est vertical ou horizontal
  int d;
  int start;
  int end;
  if (x1 == x2)
  {
    d = y1 > y2 ? -1 : 1;
    start = y1;
    end = y2 + d;
  }
  else
  {
    d = x1 > x2 ? -1 : 1;
    start = x1;
    end = x2 + d;
  }
  for (int i = start; i != end; i += d)
  {
    if (x1 == x2)
    {
      map[i][x1] = '#';
    }
    else
    {
      map[y1][i] = '#';
    }
  }
}

// Trouve les valeurs minimales et maximales de x et y (utiles pour l'affichage de la map, mais le maxY est le plus important pour le reste de l'algorithme)
void checkMin(int x, int y)
{
  if (x > maxX)
    maxX = x;
  if (x < minX)
    minX = x;
  if (y > maxY)
    maxY = y;
  if (y < minY)
    minY = y;
}

// Effectue la simulation de la descente du sable.
bool simulateSand(int x, int y)
{
  // On continue de descendre tant que l'élément n'est pas un obstacle (mur ou autre unité de sable).
  while (map[y][x] == '.')
  {
    y++;
    if (y >= MAP_SIZE)
    {
      return false;
    }
  }
  // Si un espace vide se trouve à gauche, la simulation recommence à partir de cet espace.
  if (map[y][x - 1] == '.')
  {
    return simulateSand(x - 1, y);
  }
  // Si un espace vide se trouve à droite, la simulation recommence à partir de cet espace.
  else if (map[y][x + 1] == '.')
  {
    return simulateSand(x + 1, y);
  }
  // Sinon, l'unité de sable a trouvé sa place.
  else
  {
    y--;
    map[y][x] = 'o';
    checkMin(x, y);
    return true;
  }
}

int main()
{
  // Initialise la map vide
  for (int y = 0; y < MAP_SIZE; y++)
  {
    for (int x = 0; x < MAP_SIZE; x++)
    {
      map[y][x] = '.';
    }
  }
  checkMin(500, 0);

  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day14\\input.txt";
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
    return 1;
  }
  string line;
  while (getline(file, line))
  {
    vector<string> lineSplit = splitString(line, " -> ");
    for (int i = 1; i < lineSplit.size(); i++)
    {
      int *coord1 = getCoord(lineSplit[i - 1]);
      int *coord2 = getCoord(lineSplit[i]);
      checkMin(coord1[0], coord1[1]);
      checkMin(coord2[0], coord2[1]);
      addLine(coord1[0], coord1[1], coord2[0], coord2[1]);
      delete[] coord1;
      delete[] coord2;
    }
  }
  file.close();
  showMap("Map avant la simulation");
  int countP1 = 0;
  while (simulateSand(500, 0))
  {
    countP1++;
  }
  int countP2 = countP1;
  showMap("\nMap apres la partie 1");
  addLine(0, maxY + 2, 999, maxY + 2);
  checkMin(minX, maxY + 2);
  while (map[0][500] != 'o')
  {
    simulateSand(500, 0);
    countP2++;
  }
  showMap("\nMap apres la partie 2");
  cout << "Part1: " << countP1 << "\n";
  cout << "Part2: " << countP2 << "\n";
  return 0;
}