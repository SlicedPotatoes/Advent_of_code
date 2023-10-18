/*
  L'input est une liste où chaque ligne représente un Sensor avec ses coordonnées X Y, ainsi que le Beacon le plus proche du Sensor avec ses coordonnées
  Dans la partie1, l'objectif  est de compter le nombre de positions qui ne peuvent pas contenir de Beacon pour la ligne Y = 2 000 000
  Dans la partie2, l'objectif  est de trouver dans la map (de X et Y = 0 jusqu'a X et Y = 4 000 000) la seul case qui n'est pas couverte par la zone des sensors

  Problèmes rencontrés:
  - La map étant trop grande, il fallait trouver une solution pour ne pas avoir à la stocker en mémoire.
  - Pour la partie 2, la difficulté résidait dans la recherche d'une solution suffisamant rapide, car une approche par brute force de l'ensemble de la map était trop lente.
      Je suis parti du principe que puisqu'il n'y a qu'une seule case vide, elle est forcément à une distance de 1 de la zone de couverture d'un Sensor, ce qui limite considérablement le nombre de cases à vérifier
*/
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cmath>
#include <unordered_map>

using namespace std;

struct Point
{
  int x, y;
};

// Fonction qui calcule la distance de Manhattan entre 2 points
int getDistance(Point p1, Point p2)
{
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

/*
  La classe "Input" est conçue pour extraire et stocker les informations contenue dans l'input.
  Elle calcule également la distance entre les coordonnées et les balises, ainsi que les voisins de bordure.
*/
class Input
{
public:
  Point coord;                  // Coordonnées du Sensor.
  Point balise;                 // Coordonnées de la balise la plus proche du Sensor.
  int distance;                 // Distance entre le point et le Sensor.
  vector<Point> neighborBorder; // Liste des voisins de bordure du Sensor.
  Input(string s)
  {
    // Utilisation d'un regex pour extraire les coordonnées et les balises à partir de la chaîne de texte.
    regex pattern("x=(-?\\d+), y=(-?\\d+)");
    sregex_iterator it(s.begin(), s.end(), pattern);
    sregex_iterator end;
    int i = 0;
    while (it != end)
    {
      smatch match = *it;
      int x = stoi(match[1]);
      int y = stoi(match[2]);
      it++;
      if (i == 0)
      {
        coord = {x, y};
        i++;
      }
      else
      {
        balise = {x, y};
      }
    }

    // Calcul de la distance entre les coordonnées et la balise.
    distance = getDistance(coord, balise);

    // Définition des voisins de bordure en fonction des coordonnées et de la distance.
    const int browseHelper[4][3] = {
        {coord.x - distance - 1, -1, 1},
        {coord.x, 1, 1},
        {coord.x + distance + 1, 1, -1},
        {coord.x, -1, -1}};
    int y = coord.y - distance - 1;
    int x = coord.x;
    for (int i = 0; i < 4; i++)
    {
      while (x != browseHelper[i][0])
      {
        neighborBorder.push_back({x, y});
        x += browseHelper[i][1];
        y += browseHelper[i][2];
      }
    }
  }
};

const int TARGET_ROW = 2000000;  // La ligne cible pour la partie 1
const int TARGET_ZONE = 4000000; // La zone max pour la partie 2
unordered_map<int, bool> MAP;    // Stocke seulement la ligne pour la partie 1
int countP1 = 0;

/*
  Cette fonction parcourt les voisins de bordure d'un Sensor jusqu'à trouver un Point dont la coordonnée Y est égale à TARGET_ROW
  Si un tel point est trouvé, elle parcourt horizontalement en incrémentant X (à partir de p.x + 1) jusqu'à ce que la distance au Sensor soit dépassée. Elle incrémente alors une variable si cette position n'a pas déjà été vérifiée (en utilisant MAP).
*/
void sensorCoverage(Input in)
{
  for (int i = 0; i < in.neighborBorder.size(); i++)
  {
    Point p = in.neighborBorder[i];
    if (p.y == TARGET_ROW)
    {
      int x = p.x + 1;
      while (getDistance(in.coord, {x, TARGET_ROW}) <= in.distance)
      {
        if (MAP.find(x) == MAP.end())
        {
          countP1++;
          MAP[x] = true;
        }
        x++;
      }
    }
  }
}

// Cette fonctions prend en entrée une liste de Sensor. Elle recherche et renvoie une clé unique représentant la position qui n'est pas couverte par les Sensors
long long part2(vector<Input> inputs)
{
  for (int indexInput = 0; indexInput < inputs.size(); indexInput++)
  {
    Input in = inputs[indexInput];
    for (int indexNeighbor = 0; indexNeighbor < in.neighborBorder.size(); indexNeighbor++)
    {
      Point neighbor = in.neighborBorder[indexNeighbor];
      // Vérifie que le voisin se trouve dans la zone cible délimitée par TARGET_ZONE.
      if (neighbor.x >= 0 && neighbor.x <= TARGET_ZONE && neighbor.y >= 0 && neighbor.y <= TARGET_ZONE)
      {
        bool valid = true;
        // Vérifie que le voisin ne se trouve pas dans la zone de couverture d'un autre capteur.
        for (int i = 0; i < inputs.size(); i++)
        {
          if (getDistance(neighbor, inputs[i].coord) <= inputs[i].distance)
          {
            valid = false;
            break;
          }
        }
        // Si le voisin est valide (non couvert par un autre capteur), renvoie sa position comme résultat.
        if (valid)
        {
          cout << "X: " << neighbor.x << " Y: " << neighbor.y << " RESULT: ";
          return (long long)neighbor.x * (long long)4000000 + (long long)neighbor.y;
        }
      }
    }
  }
  // Si aucune position valide n'est trouvée, retourne -1.
  return -1;
}

int main()
{
  vector<Input> inputs;
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
    Input a(line);
    inputs.push_back(a);
  }
  file.close();

  for (int i = 0; i < inputs.size(); i++)
  {
    Input in = inputs[i];

    if (in.balise.y == TARGET_ROW)
    {
      MAP[in.balise.x] = true;
    }

    sensorCoverage(in);
  }

  cout << "Part1: " << countP1 << "\n";
  cout << "Part2: " << part2(inputs) << "\n";
  return 0;
}