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
  int win[3] = {2, 0, 1};
  int lose[3] = {1, 2, 0};
  while (getline(file, line))
  {
    int enemy = line[0] - 'A';
    int me = line[2] - 'X';
    if (enemy == win[me])
    {
      scorePart1 += 6;
    }
    else if (enemy == me)
    {
      scorePart1 += 3;
    }
    scorePart1 += me + 1;
    if (me == 1)
    {
      scorePart2 += 3;
      me = enemy;
    }
    else if (me == 2)
    {
      scorePart2 += 6;
      me = lose[enemy];
    }
    else
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