/*
  L'entrée consiste en une chaîne de caractères composée de '(' et ')' représentant les instructions à suivre dans un
  immeuble.

  Un '(' indique de monter d'un étage, tandis qu'une ')' signifie de descendre d'un étage.

  Dans la première partie, l'objectif est de déterminer à quel étage nous nous trouvons à la fin des instructions.
  Dans la deuxième partie, l'objectif est de trouver à quelle instruction nous atteignons le sous-sol (étage -1).
*/
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int part1(string s)
{
    int floor = 0;

    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == '(')
        {
            floor++;
        }
        else if (s[i] == ')')
        {
            floor--;
        }
    }

    return floor;
}

int part2(string s)
{
    int floor = 0;

    for (size_t i = 0; i < s.size(); i++)
    {
        if (s[i] == '(')
        {
            floor++;
        }
        else if (s[i] == ')')
        {
            floor--;
            if (floor == -1)
            {
                return i + 1;
            }
        }
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
    file.close();

    cout << "Part1: " << part1(line) << '\n';
    cout << "Part2: " << part2(line) << '\n';

    return 0;
}