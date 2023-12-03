/*
  L'entrée représente le "schéma technique" d'une télécabine.

  Dans la première partie, l'objectif est de calculer la somme des numéros des pièces de la machine.
  Pour trouver une pièce dans le schéma, il faut respecter les conditions suivantes:
  - Tout nombre adjacent à un symbole est un numéro de pièce.
  - Un '.' n'est pas un symbole mais une case vide.

  Pour la deuxième partie, l'objectif est de calculer la somme du "rapport de démultiplication" des engrenages.
  Un engrenage doit respecter les conditions suivantes :
  - Un engrenage a pour symbole '*'.
  - Il doit avoir exactement 2 nombres adjacents au symbole.
  Le "rapport de démultiplication" d'un engrenage est le produit des 2 nombres adjacents.
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <D:\Users\Kevin\Desktop\Repo\Advent_of_code\Utils\MeasureExecutionTime.cpp>

using namespace std;

struct Point
{
    int x, y;
    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }
};

vector<string> schematic;

vector<Point> dir = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

int parseInput()
{
    MEASURE_FUNCTION_EXECUTION_TIME
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
        schematic.push_back(line);
    }
    file.close();
    return 0;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int maxY = schematic.size();
    int maxX = schematic[0].size();
    int sum = 0;

    // Parcourir les lignes du schéma
    for (int y = 0; y < maxY; y++)
    {
        // Parcourir les colonnes du schéma
        for (int x = 0; x < maxX; x++)
        {
            // Si le caractère à la position (x, y) est un chiffre
            if (isdigit(schematic[y][x]))
            {
                // Recherche de la fin du nombre
                int endX = x;
                while (endX < maxX && isdigit(schematic[y][endX]))
                {
                    endX++;
                }

                // Vérification de l'adjacence avec un caractère autre que '.'
                bool isValid = false;
                for (int _x = x; _x < endX; _x++)
                {
                    for (const Point &d : dir)
                    {
                        int adjX = _x + d.x;
                        int adjY = y + d.y;

                        if (adjX >= 0 && adjX < maxX && adjY >= 0 && adjY < maxY && !isdigit(schematic[adjY][adjX]) &&
                            schematic[adjY][adjX] != '.')
                        {
                            isValid = true;
                            break;
                        }
                    }
                }

                // Ajout du nombre à la somme si les conditions sont remplies
                if (isValid)
                {
                    sum += stoi(schematic[y].substr(x, endX - x));
                }

                // Mise à jour de la position x pour éviter de traiter à nouveau le même nombre
                x = endX;
            }
        }
    }
    return sum;
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int maxY = schematic.size();
    int maxX = schematic[0].size();
    int sum = 0;

    // Parcourir les lignes du schéma
    for (int y = 0; y < maxY; y++)
    {
        // Parcourir les colonnes du schéma
        for (int x = 0; x < maxX; x++)
        {
            // Si le caractère à la position (x, y) est '*'
            if (schematic[y][x] == '*')
            {
                vector<Point> pointIsInNumbers; // Liste des points qui sont dans des nombres adjacents deja trouvé
                vector<int> numbers;            // Liste des nombres adjacents
                // Parcourir les cases adjacentes
                for (Point d : dir)
                {
                    Point p = {x + d.x, y + d.y};
                    // Vérifier si la case adjacente contient un nombre
                    if (p.x >= 0 && p.x < maxX && p.y >= 0 && p.y < maxY && isdigit(schematic[p.y][p.x]))
                    {
                        // On vérifie que le nombre trouvé n'a pas déjà été trouvé avant
                        if (find(pointIsInNumbers.begin(), pointIsInNumbers.end(), p) == pointIsInNumbers.end())
                        {
                            // Récupérer le nombre
                            string number = "";
                            int _x = p.x;
                            while (_x >= 0 && isdigit(schematic[p.y][_x]))
                            {
                                number = schematic[p.y][_x] + number;
                                pointIsInNumbers.push_back({_x, p.y});
                                _x--;
                            }
                            _x = p.x + 1;
                            while (_x < maxX && isdigit(schematic[p.y][_x]))
                            {
                                number += schematic[p.y][_x];
                                pointIsInNumbers.push_back({_x, p.y});
                                _x++;
                            }
                            numbers.push_back(stoi(number));
                        }
                    }
                }
                // Si il y a exactement 2 nombres, ajouter le produit de ces nombres à la somme
                if (numbers.size() == 2)
                {
                    sum += numbers[0] * numbers[1];
                }
            }
        }
    }

    return sum;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();
    int part2 = processPart2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}