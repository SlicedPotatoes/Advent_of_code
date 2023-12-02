/*
  L'entrée représente une liste d'instructions, chaque instruction possède les informations suivantes:
  - Deux coordonnées X et Y, représentant respectivement le coin supérieur gauche et le coin inférieur droit d'un
  rectangle.
  - Une indication indiquant s'il faut allumer, éteindre ou inverser l'état.

  Les coordonnées sont comprises entre 0 et 999.

  Dans la première partie, l'objectif est de suivre les instructions à la lettre à partir d'une grille où tout est
  éteint.
  Nous devons retourner le nombre de lumières allumées à la fin des instructions.

  Dans la deuxième partie, nous partons d'une grille remplie de zéros, chaque valeur représente la luminosité.
  - Pour l'instruction "turn on" (allumer) : nous incrémentons la luminosité de 1.
  - Pour l'instruction "turn off" (éteindre) : nous décrémentons la luminosité de 1 tout en vérifiant qu'elle ne devient
  pas négative.
  - Pour l'instruction "toggle" (inverser) : nous incrémentons la luminosité de 2.
  À la fin des instructions, nous devons retourner la luminosité totale.
*/

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

const int GRID_SIZE = 1000;

bool gridP1[GRID_SIZE][GRID_SIZE];
int gridP2[GRID_SIZE][GRID_SIZE];

void process(int startX, int startY, int endX, int endY, int value)
{
    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (value == -1)
            {
                gridP1[y][x] = !gridP1[y][x];
                gridP2[y][x] += 2;
            }
            else if (value == 0)
            {
                gridP1[y][x] = false;
                gridP2[y][x]--;
                if (gridP2[y][x] < 0)
                {
                    gridP2[y][x] = 0;
                }
            }
            else
            {
                gridP1[y][x] = true;
                gridP2[y][x]++;
            }
        }
    }
}

int main()
{
    for (size_t y = 0; y < GRID_SIZE; y++)
    {
        for (size_t x = 0; x < GRID_SIZE; x++)
        {
            gridP1[y][x] = false;
            gridP2[y][x] = 0;
        }
    }

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
        regex pattern("\\D*(\\d+)\\D*(\\d+)\\D*(\\d+)\\D*(\\d+)\\D*");

        smatch match;
        if (regex_search(line, match, pattern))
        {
            int startX = stoi(match[1]);
            int startY = stoi(match[2]);
            int endX = stoi(match[3]);
            int endY = stoi(match[4]);

            int value = line.substr(0, 6) == "toggle" ? -1 : (line.substr(5, 2) == "on" ? 1 : 0);

            process(startX, startY, endX, endY, value);
        }
    }
    file.close();

    int part1 = 0;
    int part2 = 0;

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (gridP1[y][x])
            {
                part1++;
            }
            part2 += gridP2[y][x];
        }
    }

    cout << "Part1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}