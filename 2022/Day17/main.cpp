/*
  L'objectif de cet exercice est de simuler la chute de rochers de différentes formes. Ces rochers sont soumis à des déplacements latéraux pendant leur chute qui est représenté par l'input.

  Les différentes formes de rochers sont les suivantes :

  ####    .#.      ..#      #      ##
          ###      ..#      #      ##
          .#.      ###      #
                            #

  Les rochers tombent dans l'ordre indiqué ci-dessus, et une fois la fin de la liste atteinte, le même ordre se répète. La simulation se déroule comme suit :

  - Le rocher est initialisé à une hauteur de 3 unités au-dessus du premier obstacle (sol ou autres rochers) et à 2 unités du bord gauche.
  - Le rocher est déplacé latéralement une fois en fonction de l'entrée (si la fin de la liste est atteinte, elle est répétée). Si le déplacement est impossible, on continue sans le compter.
  - On descend d'une unité.
  - On répète les deux opérations ci-dessus jusqu'à ce qu'un obstacle soit rencontré vers le bas.

  Pour la partie 1, il faut simuler jusqu'au rocher 2022.
  Pour la partie 2, il faut simuler jusqu'au rocher 10^12.

  Dans les deux parties, la sortie attendue est la hauteur de la tour formée par la chute.

  La difficulté de la partie 2 réside dans la nécessité d'exécuter un nombre extrêmement élevé de chutes de rochers.
  L'idée que j'ai eue est de trouver des séquences répétitives et de les exploiter pour éviter de calculer de manière itérative la hauteur.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <chrono>

using namespace std;

// Constantes pour les limites et les marges.
const long long PART_1_LIMIT = 2022;          // Limite de la partie 1.
const long long PART_2_LIMIT = 1000000000000; // Limite de la partie 2.
const int LEFT_PADDING = 2;                   // Marge de gauche pour l'apparition d'une pièce.
const int BOTTOM_PADDING = 3;                 // Marge inférieure pour l'apparition  d'une pièce.
const int MAP_WIDTH = 7;                      // Largeur de la map.
const int TOP_MAP_SIZE = 50;                  // Hauteur de la map depuis le haut prise en compte pour générer les clés de détection de séquence.

array<bool, MAP_WIDTH> DEFAULT_LINE_MAP = {false, false, false, false, false, false, false}; // Ligne par défaut pour la map.

// Structure de stockage d'un modèle de rocher.
struct RockPattern
{
  vector<array<int, 2>> pattern; // Motif du rocher.
  int height;                    // Hauteur du rocher.
  int width;                     // Largeur du rocher.
};

string input = ""; // Stockage de l'entrée depuis le fichier input.txt.

// Liste des modèles de rochers.
vector<RockPattern> rockPatterns = {
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, 1, 4},
    {{{0, 1}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 1}}, 3, 3},
    {{{0, 2}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}}, 3, 3},
    {{{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, 4, 1},
    {{{0, 0}, {0, 1}, {-1, 0}, {-1, 1}}, 2, 2}};

vector<array<bool, MAP_WIDTH>> _map;        // Carte de la simulation.
long long maxHeightMap = 0;                 // Hauteur maximale actuelle de la carte.
int gasIndex = 0;                           // Indice des déplacements latéraux.
map<string, array<long long, 2>> sequences; // Map pour stocker les clés représentant l'état de la chute à un instant donné afin de détecter des séquences répétitives.
long long rowProcessWithSequence = 0;       // Hauteur de la carte calculée à partir des séquences.

// Vérifie si le rocher peut être déplacé aux coordonnées (x, y) sans collision.
bool canBeHere(int x, long long y, RockPattern rockPattern)
{
  for (int i = 0; i < rockPattern.pattern.size(); i++)
  {
    int _y = y + rockPattern.pattern[i][0];
    int _x = x + rockPattern.pattern[i][1];
    // Vérifie les limites de la carte et les collisions.
    if (!(_x >= 0 && _x < MAP_WIDTH) || !(_y >= 0 && _y < _map.size()) || _map[_y][_x])
    {
      return false;
    }
  }
  return true;
}
// Simule les déplacements latéraux des rochers.
int simulateGas(int x, int y, RockPattern rockPattern)
{
  int _x = x; // Copie temporaire de x.

  // Applique le déplacement en fonction de l'indice.
  switch (input[gasIndex])
  {
  case '<':
    _x--;
    break;
  case '>':
    _x++;
    break;
  }
  // Si le déplacement est possible, met à jour x avec la copie temporaire.
  if (canBeHere(_x, y, rockPattern))
  {
    x = _x;
  }
  gasIndex++; // Incrémente l'indice.
  // Si l'indice atteint la fin de la liste, le ramène à 0.
  if (gasIndex == input.size())
  {
    gasIndex = 0;
  }
  return x; // Retourne la nouvelle valeur de x.
}
// Simule la chute d'un rocher.
void simulateOneRock(long long i)
{
  // Récupère le motif du rocher à traiter.
  RockPattern rockPattern = rockPatterns[i % rockPatterns.size()];

  // Ajoute des lignes vides à la carte si nécessaire.
  while (_map.size() < maxHeightMap + BOTTOM_PADDING + rockPattern.height)
  {
    _map.push_back(DEFAULT_LINE_MAP);
  }

  // Initialise les coordonnées du rocher.
  int x = LEFT_PADDING;
  long long y = maxHeightMap + BOTTOM_PADDING + rockPattern.height - 1;

  // Simule les déplacements latéraux et la chute jusqu'à rencontrer un obstacle vers le bas.
  while (true)
  {
    x = simulateGas(x, y, rockPattern);
    if (!canBeHere(x, y - 1, rockPattern))
    {
      break;
    }
    y--;
  }

  // Met à jour la carte.
  for (int j = 0; j < rockPattern.pattern.size(); j++)
  {
    int _y = y + rockPattern.pattern[j][0];
    int _x = x + rockPattern.pattern[j][1];
    _map[_y][_x] = true;
  }

  // Met à jour la hauteur maximale actuelle.
  if (y + 1 > maxHeightMap)
  {
    maxHeightMap = y + 1;
  }
}
// Retourne une chaîne de caractères représentant la partie supérieure de la carte.
string getTopMapString()
{
  // Si la carte est plus petite que la hauteur prise en compte, renvoie sa taille.
  if (_map.size() <= TOP_MAP_SIZE)
  {
    return to_string(_map.size());
  }

  string s = "";
  // Construit la chaîne de caractères à partir des lignes supérieures de la carte.
  for (int i = _map.size() - 1; i >= _map.size() - TOP_MAP_SIZE; i--)
  {
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      s += (_map[i][j] ? "#" : ".");
    }
  }
  return s;
}
// Calcule la hauteur d'une séquence et renvoie le nombre de rochers qu'elle comporte.
long long processWithSequence(long long i, string keySequencePrec, long long nbConsecutiveCalls)
{
  /*
    Récupère les informations de l'état de la carte déjà rencontré.
    - index 0 = MaxHeight
    - index 1 = i
  */
  array<long long, 2> dataTargetSequence = sequences[keySequencePrec];

  // Le nombre de rochers dans cette séquence est la différence entre le nombre actuel de rochers et le nombre de rochers en bas de la séquence.
  long long nbRockSequence = i - dataTargetSequence[1];

  // Si l'ajout de cette séquence dépasse les limites de la partie 1 ou 2, on la saute.
  if ((i < PART_1_LIMIT && i + nbRockSequence >= PART_1_LIMIT) || (i >= PART_1_LIMIT && i + nbRockSequence >= PART_2_LIMIT))
  {
    return 0;
  }

  // La hauteur de cette séquence est la différence entre la hauteur actuelle de la carte et la hauteur de la carte lors de l'état précédent, multipliée par 2^nbConsecutiveCalls.
  long long nbRowSequence = (maxHeightMap - dataTargetSequence[0]) * pow(2, nbConsecutiveCalls);
  // Incrémente la hauteur calculée grâce à la séquence.
  rowProcessWithSequence += nbRowSequence;

  // Retourne le nombre de rochers dans cette séquence.
  return nbRockSequence;
}
// Affiche le résultat de la simulation.
void showResult(string title, chrono::_V2::system_clock::time_point _start)
{
  auto stop = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(stop - _start);
  cout << "MaxHeightMap: " << maxHeightMap << "\n";
  cout << "RowProcessWithSequence: " << rowProcessWithSequence << "\n";
  cout << title << maxHeightMap + rowProcessWithSequence << "\n";
  cout << "Temps d'exécution : " << time.count() << " μs\n";
  cout << "\n";
}
// Simule et détecte les séquences.
void simulate(int start, long long end)
{
  auto _start = chrono::high_resolution_clock::now();
  for (long long i = start; i < end; i++)
  {
    // Simule la chute d'un rocher.
    simulateOneRock(i);

    // Crée une clé représentant l'état actuel.
    string key = getTopMapString() + ':' + to_string(i % rockPatterns.size()) + ':' + to_string(gasIndex);

    // Si cet état a déjà été rencontré précédemment.
    if (sequences.find(key) != sequences.end())
    {
      long long nbConsecutiveCalls = 0;
      while (true)
      {
        // Calcule la hauteur et le nombre de rochers de la séquence détectée.
        long long nbRock = processWithSequence(i, key, nbConsecutiveCalls);

        i += nbRock; // Incrémente l'indice en sautant la séquence.
        if (nbRock == 0)
        {
          break;
        }
        nbConsecutiveCalls++;
      }
      sequences.clear(); // Réinitialise la liste des séquences après les avoir traitées.
    }
    sequences[key] = {maxHeightMap, i}; // Stocke l'état actuel avec sa hauteur maximale dans les séquences.
    if (i == PART_1_LIMIT - 1)
    {
      showResult("Part1: ", _start); // Affiche le résultat de la partie 1 si l'indice atteint la limite de la partie 1.
    }
  }
  showResult("Part2: ", _start); // Affiche le résultat de la partie 2 à la fin de la simulation.
}

main()
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
    input += line;
  }
  file.close();

  simulate(0, PART_2_LIMIT);

  return 0;
}