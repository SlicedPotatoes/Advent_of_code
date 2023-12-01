/*
  L'entrée représente une liste de dimensions de paquets cadeaux rectangulaires.

  Pour la première partie, le calcul consiste à déterminer la somme totale de papier cadeau nécessaire pour emballer
  tous les cadeaux.
  La surface totale est définie comme étant égale à 2*longeur*largeur + 2*largeur*hauteur + 2*hauteur*longeur.
  Pour garantir une marge suffisante, on ajoute la surface du paquet cadeau la plus petite, par exemple:
  longeur=2 largeur=3 hauteur=4, la plus petite surface est longueur*largeur

  Pour la deuxième partie, le calcul vise à déterminer la longueur de ruban nécessaire pour finaliser l'emballage des
  paquets cadeaux.
  La longueur du ruban correspond à la plus courte distance autour de ses côtés, par exemple:
  pour longeur=2 largeur=3 hauteur=4 la plus petite distance est longeur+largeur+longeur+largeur
   Ensuite, il est nécessaire d'ajouter la longueur du nœud du ruban, égale à longeur*largeur*hauteur
*/

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Retourne un vecteur de int en fonction du string d'entrée, ce vecteur est trié par ordre croissant.
vector<int> splitString(const string &input, const string &delimiter)
{
    vector<int> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        if (end != start)
        {
            tokens.push_back(stoi(input.substr(start, end - start)));
        }
        start = end + delimiter.length();
    }

    if (start < input.length())
    {
        tokens.push_back(stoi(input.substr(start)));
    }
    sort(tokens.begin(), tokens.end());
    return tokens;
}

int main()
{
    string filename = "input.txt";
    ifstream file(filename);

    int part1 = 0;
    int part2 = 0;

    if (!file.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
        return 1;
    }
    string line;
    while (getline(file, line))
    {
        vector<int> splitedInput = splitString(line, "x");
        int l = splitedInput[0];
        int w = splitedInput[1];
        int h = splitedInput[2];

        int LxW = l * w;
        int WxH = w * h;
        int HxL = h * l;

        int surface = 2 * LxW + 2 * WxH + 2 * HxL + LxW;
        int ruban = 2 * l + 2 * w + l * w * h;

        part1 += surface;
        part2 += ruban;
    }
    file.close();

    cout << "Part1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}