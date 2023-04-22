#include <iostream>
#include <fstream>
#include <string>

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
  while (getline(file, line))
  {
  }
  file.close();

  return 0;
}