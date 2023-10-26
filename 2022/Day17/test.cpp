#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <chrono>

using namespace std;

int checkSequence(vector<string> sequence, vector<string> map)
{
  bool findSequence = false;
  bool endCountSequence = false;
  int countSequence = 0;
  int tempCountSequence = 0;
  for (int i = 0; i < map.size(); i++)
  {
    if (findSequence && !endCountSequence)
    {
      countSequence++;
    }
    else if (endCountSequence)
    {
      tempCountSequence++;
    }
    if (map[i] == sequence[0])
    {
      bool valid = true;
      for (int j = 1; j < sequence.size(); j++)
      {
        if (map[i + j] != sequence[j])
        {
          valid = false;
          break;
        }
      }
      if (!findSequence && valid)
      {
        findSequence = true;
      }
      else if (findSequence && !endCountSequence && valid)
      {
        endCountSequence = true;
      }
      else if (valid && endCountSequence)
      {
        if (countSequence == tempCountSequence)
        {
          tempCountSequence = 0;
        }
        else
        {
          return -1;
        }
      }
    }
  }
  if (!findSequence)
  {
    return -1;
  }
  if (!endCountSequence)
  {
    return -1;
  }
  return countSequence;
}

int main()
{
  const int SEQUENCE_SIZE = 5;
  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day17\\output.txt";
  ifstream file(filename);

  vector<string> v;

  if (!file.is_open())
  {
    cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
    return 1;
  }
  string line;
  while (getline(file, line))
  {
    v.push_back(line);
  }
  file.close();

  cout << checkSequence({"..####.", "..###..", "..###..", "..####."}, v) << "\n";

  cout << checkSequence({"..#.#..", "#####..", "..###..", "...#...", "..####."}, v) << "\n";

  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < v.size(); i++)
  {
    vector<string> tempSequence;
    for (int j = 0; j < SEQUENCE_SIZE; j++)
    {
      tempSequence.push_back(v[i + j]);
    }
    int s = checkSequence(tempSequence, v);
    if (s != -1)
    {
      cout << s << "\n";
      for (int j = 0; j < tempSequence.size(); j++)
      {
        cout << tempSequence[j] << "\n";
      }
      cout << i << "\n";
      break;
    }
  }
  auto stop = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << "Temps d'exécution : " << time.count() << " μs\n";

  return 0;
}