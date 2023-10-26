#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#include <chrono>

using namespace std;

const int PART_1_LIMIT = 2022;
const long long PART_2_LIMIT = 10LL * 1000 * 1000 * 1000 * 1000;
const int RIGHT_PADDING = 2;
const int BOTTOM_PADDING = 3;
const int MAP_WIDTH = 7;
const int SEQUENCE_SIZE = 5;
array<bool, MAP_WIDTH> DEFAULT_LINE_MAP = {false, false, false, false, false, false, false};

struct RockPattern
{
  vector<array<int, 2>> pattern;
  int height;
  int width;
};
struct Sequence
{
  vector<array<bool, MAP_WIDTH>> s;
  int offset;
  int startIndex;
};

string input = "";
vector<RockPattern> rockPatterns = {
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}, 1, 4},
    {{{0, 1}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 1}}, 3, 3},
    {{{0, 2}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}}, 3, 3},
    {{{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, 4, 1},
    {{{0, 0}, {0, 1}, {-1, 0}, {-1, 1}}, 2, 2}};

vector<array<bool, MAP_WIDTH>> map;
long long maxHeightMap = 0;
int gasIndex = 0;

Sequence sequencePart2 = {{}, -1, -1};

void showMap()
{
  for (int i = map.size() - 1; i >= map.size() - 11; i--)
  {
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      cout << (map[i][j] ? "#" : ".");
    }
    cout << "\n";
  }
}

bool canBeHere(int x, long long y, RockPattern rockPattern)
{
  for (int i = 0; i < rockPattern.pattern.size(); i++)
  {
    int _y = y + rockPattern.pattern[i][0];
    int _x = x + rockPattern.pattern[i][1];
    if (!(_x >= 0 && _x < MAP_WIDTH) || !(_y >= 0 && _y < map.size()) || map[_y][_x])
    {
      return false;
    }
  }
  return true;
}

bool isSameLine(array<bool, MAP_WIDTH> a, array<bool, MAP_WIDTH> b)
{
  for (int i = 0; i < MAP_WIDTH; i++)
  {
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

bool startMapIsSequence()
{
  int indexStartRock = map.size() - 1;
  while (isSameLine(DEFAULT_LINE_MAP, map[indexStartRock]))
  {
    indexStartRock--;
  }
  for (int i = 0; i < sequencePart2.s.size(); i++)
  {
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      cout << (sequencePart2.s[sequencePart2.s.size() - 1 - i][j] ? "#" : "*");
    }
    cout << "\n";
    for (int j = 0; j < MAP_WIDTH; j++)
    {
      cout << (map[indexStartRock - i][j] ? "#" : "*");
    }
    cout << "\n\n";
    if (!isSameLine(sequencePart2.s[sequencePart2.s.size() - 1 - i], map[indexStartRock - i]))
    {
      return false;
    }
  }
  return true;
}

void simulateOneRock(int i)
{
  RockPattern rockPattern = rockPatterns[i % rockPatterns.size()];
  while (map.size() < maxHeightMap + BOTTOM_PADDING + rockPattern.height)
  {
    map.push_back(DEFAULT_LINE_MAP);
  }
  int x = RIGHT_PADDING;
  long long y = maxHeightMap + BOTTOM_PADDING + rockPattern.height - 1;

  bool gasPush = true;
  bool end = false;

  while (!end)
  {
    if (gasPush)
    {
      int _x = x;
      switch (input[gasIndex])
      {
      case '<':
        _x--;
        break;
      case '>':
        _x++;
        break;
      }
      if (canBeHere(_x, y, rockPattern))
      {
        x = _x;
      }
      gasIndex++;
      if (gasIndex == input.size())
      {
        gasIndex = 0;
      }
    }
    else
    {
      if (!canBeHere(x, y - 1, rockPattern))
      {
        end = true;
        break;
      }
      y--;
    }
    gasPush = !gasPush;
  }
  for (int j = 0; j < rockPattern.pattern.size(); j++)
  {
    int _y = y + rockPattern.pattern[j][0];
    int _x = x + rockPattern.pattern[j][1];
    map[_y][_x] = true;
  }
  if (y + 1 > maxHeightMap)
  {
    maxHeightMap = y + 1;
  }
}

int simulateUntilFoundSequence()
{
  int i = PART_1_LIMIT;
  while (!startMapIsSequence())
  {
    simulateOneRock(i);
    i++;
  }
  return i - PART_1_LIMIT;
}

void simulateP1()
{
  for (int i = 0; i < PART_1_LIMIT; i++)
  {
    simulateOneRock(i);
  }
}

/*
  Les fonctions qui permettrons de detecté un pattern pour optimisé la partie 2
*/

int checkSequence(vector<array<bool, MAP_WIDTH>> sequence)
{
  bool foundSequence = false;    // True si la sequence a été trouvé dans la map
  bool endCountSequence = false; // True si on a retrouvé la meme sequence une 2eme
  int countSequence = 0;         // Compte la différence de ligne entre la premiere fois que l'on a trouvé la sequence et la seconde fois
  int tempCountSequence = 0;     // Compteur temporaire pour les autres fois que l'on trouve la sequence pour vérifier que l'ecart est le meme.

  // Parcours de la map
  for (int i = 0; i < map.size(); i++)
  {
    // Si la sequence a été trouvé mais que l'on a pas trouvé la seconde, on incrémente le compteur.
    if (foundSequence && !endCountSequence)
    {
      countSequence++;
    }
    // Si on a trouvé la seconde séquence, on incrémente le compteur temporaire
    else if (endCountSequence)
    {
      tempCountSequence++;
    }
    // Si la ligne de la carte est = a la premiere ligne de la sequence
    if (isSameLine(map[i], sequence[0]))
    {
      // On verifie si la sutie correspond
      bool valid = true;
      for (int j = 1; j < sequence.size(); j++)
      {
        if (!isSameLine(map[i - j], sequence[j]))
        {
          valid = false;
          break;
        }
      }
      // Si notre sequence est valid et que l'on ne l'avais deja pas trouvé, on met la valeur de foundSequence sur true
      if (!foundSequence && valid)
      {
        foundSequence = true;
      }
      // Si c'est la 2eme fois que l'on trouve la sequence, on met endCountSequence sur true
      else if (foundSequence && !endCountSequence && valid)
      {
        endCountSequence = true;
      }
      // Si on a retrouvé la sequence et que c'est ni la premiere ni la seconde fois
      else if (valid && endCountSequence)
      {
        // On vérifie si l'eccart est le meme que ce que l'on avais calculé entre la premiere et la seconde
        if (countSequence == tempCountSequence)
        {
          tempCountSequence = 0;
        }
        // Si ce n'est pas le cas on retourne -1
        else
        {
          return -1;
        }
      }
    }
  }
  // Si notre sequence n'a pas été trouvé on retourne -1
  if (!foundSequence)
  {
    return -1;
  }
  // Si la 2eme sequence n'a pas été trouvé on retourne -1
  if (!endCountSequence)
  {
    return -1;
  }
  // Sinon on retourne l'espacement entre les sequences.
  return countSequence;
}

Sequence findSequence()
{
  for (int i = 0; i < map.size(); i++)
  {
    vector<array<bool, MAP_WIDTH>> tempSequence;
    for (int j = 0; j < SEQUENCE_SIZE; j++)
    {
      tempSequence.push_back(map[i + j]);
    }
    int s = checkSequence(tempSequence);
    if (s != -1)
    {
      return {tempSequence, s, i};
    }
  }
  return {{}, -1, -1};
}

main()
{
  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day17\\input.txt";
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

  auto start = chrono::high_resolution_clock::now();
  simulateP1();
  auto stop = chrono::high_resolution_clock::now();
  auto timeP1 = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << "Part1: " << maxHeightMap << "\n";
  cout << "Temps d'exécution : " << timeP1.count() << " μs\n";

  start = chrono::high_resolution_clock::now();

  sequencePart2 = findSequence();

  stop = chrono::high_resolution_clock::now();
  auto timeFindSequence = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << "sequencePart2 Temps d'exécution : " << timeFindSequence.count() << " μs\n";

  int t = simulateUntilFoundSequence();
  cout << t << "\n";

  string filename2 = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day17\\output.txt";
  ofstream fichier(filename2);
  if (fichier.is_open())
  {
    for (int i = map.size() - 1; i >= 0; i--)
    {
      for (int j = 0; j < MAP_WIDTH; j++)
      {
        fichier << (map[i][j] ? "#" : ".");
      }
      fichier << "\n";
    }
  }

  return 0;
}