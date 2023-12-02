/*
  l'entrée représente une liste de déplacements sur un plan 2D, où les symboles suivants indiquent la direction:
  - ^ : allez vers le haut
  - > : allez vers la droite
  - v : allez vers le bas
  - < : allez vers la gauche

  Pour la première partie, l'objectif est de compter le nombre unique de maisons où le Père Noël a livré des cadeaux en
  suivant les instructions.

  Dans la deuxième partie, le Père Noël est accompagné d'un assistant pour les livraisons.
  Le Père Noël suit les instructions aux indices pairs, tandis que l'assistant suit les instructions aux indices
  impairs.
  Le résultat attendu est le nombre de maisons uniques visitées par l'équipe.

  On part du principe que le point de départ avant instruction est une maison où un cadeau a été distribué.
*/
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

struct Point
{
    int x, y;

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }

    struct Hash
    {
        size_t operator()(const Point &p) const
        {
            size_t xHash = hash<int>()(p.x);
            size_t yHash = hash<int>()(p.y);

            return xHash ^ yHash;
        }
    };
};

int part1(string s)
{
    unordered_set<Point, Point::Hash> set;

    Point p = {0, 0};

    set.insert(p);

    for (size_t i = 0; i < s.size(); i++)
    {
        char instruction = s[i];
        if (instruction == '^')
        {
            p.y--;
        }
        else if (instruction == '>')
        {
            p.x++;
        }
        else if (instruction == 'v')
        {
            p.y++;
        }
        else
        {
            p.x--;
        }
        set.insert(p);
    }

    return set.size();
}

int part2(string s)
{
    Point santa = {0, 0};
    Point robotSanta = {0, 0};

    unordered_set<Point, Point::Hash> set;
    set.insert(santa);

    for (size_t i = 0; i < s.size(); i++)
    {
        char instruction = s[i];
        if (instruction == '^')
        {
            i % 2 == 0 ? santa.y-- : robotSanta.y--;
        }
        else if (instruction == '>')
        {
            i % 2 == 0 ? santa.x++ : robotSanta.x++;
        }
        else if (instruction == 'v')
        {
            i % 2 == 0 ? santa.y++ : robotSanta.y++;
        }
        else
        {
            i % 2 == 0 ? santa.x-- : robotSanta.x--;
        }
        Point p = i % 2 == 0 ? santa : robotSanta;
        set.insert(p);
    }

    return set.size();
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