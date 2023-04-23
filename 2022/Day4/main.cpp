/*
  Dans l'input, chaque ligne représente une paire de personnes (chaque personne est séparée par une virgule).
  Chaque personne de la paire est attribuée au nettoyage d'un intervalle de section.
  Dans la partie 1, on compte pour chaque paire si l'intervalle d'une personne est totalement inclus dans l'intervalle de son coéquipier.
  Dans la partie 2, on compte pour chaque paire si les intervalles se chevauchent.
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
  int countPart1 = 0;
  int countPart2 = 0;
  while (getline(file, line))
  {
    vector<string> pairs = split(line, ',');
    vector<string> range1 = split(pairs[0], '-');
    vector<string> range2 = split(pairs[1], '-');
    int intRange1[2] = {stoi(range1[0]), stoi(range1[1])};
    int intRange2[2] = {stoi(range2[0]), stoi(range2[1])};
    // Part 1
    if (intRange1[0] <= intRange2[0] && intRange1[1] >= intRange2[1])
    {
      countPart1++;
    }
    else if (intRange2[0] <= intRange1[0] && intRange2[1] >= intRange1[1])
    {
      countPart1++;
    }
    // Part 2;
    if (!((intRange1[1] < intRange2[0]) || (intRange2[1] < intRange1[0])))
    {
      countPart2++;
    }
  }
  file.close();
  cout << "Part1: " << countPart1 << '\n';
  cout << "Part2: " << countPart2;
  return 0;
}