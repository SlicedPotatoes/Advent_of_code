/*
  Chaque ligne de l'entrée représente les composants d'un jeu:
  - L'identifiant de la partie est fourni.
  - Chaque séquence, séparée par un ';', représente un tour de la partie.
  - Pour chaque tour, le nombre de cubes de chaque couleur est spécifié.

  Dans la partie 1, l'objectif est de déterminer le nombre de parties valides, sachant que le nombre initial de cubes
  est le suivant:
  - 14 cubes bleus.
  - 13 cubes verts.
  - 12 cubes rouges.
  Ensuite, il faut retourner la somme des identifiants des parties valides.

 Dans la partie 2, l'objectif est de trouver, pour chaque partie, le nombre minimum de cubes de chaque couleur
 nécessaire pour qu'elle soit valide.
 Ensuite, il faut calculer le produit des nombres minimums pour chaque couleur.
 La sortie attendue est la somme de ces produits.

*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX_BLUE = 14;
const int MAX_GREEN = 13;
const int MAX_RED = 12;

vector<string> splitString(const string &input, const string &delimiter)
{
    vector<string> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        if (end != start)
        {
            tokens.push_back(input.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    if (start < input.length())
    {
        tokens.push_back(input.substr(start));
    }

    return tokens;
}

struct Sequence
{
    int blue = 0;
    int red = 0;
    int green = 0;
};

class Input
{
  public:
    int id;
    vector<Sequence> sequences;
    Input(string s)
    {
        this->s = s;
        getID();
        getSequence();
    }

  private:
    string s;
    void getID()
    {
        size_t index = s.find(':');
        string id = s.substr(0, index);
        this->s = s.substr(index + 1);
        this->id = stoi(id.substr(5));
    }
    void getSequence()
    {
        vector<string> seqs = splitString(s, ";");
        for (string seqStr : seqs)
        {
            vector<string> colors = splitString(seqStr, ",");
            Sequence seq;
            for (string c : colors)
            {
                size_t _index = c.find(' ', 1);
                int value = stoi(c.substr(1, _index));
                if (c.find("blue") != string::npos)
                {
                    seq.blue = value;
                }
                else if (c.find("red") != string::npos)
                {
                    seq.red = value;
                }
                else if (c.find("green") != string::npos)
                {
                    seq.green = value;
                }
                else
                {
                    cerr << "Error Parsing" << seqStr << " Color: " << c << '\n';
                }
            }
            sequences.push_back(seq);
        }
    }
};

int main()
{
    int part1 = 0;
    int part2 = 0;

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
        Input i = {line};
        bool valid = true;
        int maxBlue = 0;
        int maxRed = 0;
        int maxGreen = 0;
        for (Sequence s : i.sequences)
        {
            if (s.blue > MAX_BLUE || s.green > MAX_GREEN || s.red > MAX_RED)
            {
                valid = false;
            }
            maxBlue = max(maxBlue, s.blue);
            maxRed = max(maxRed, s.red);
            maxGreen = max(maxGreen, s.green);
        }
        if (valid)
        {
            part1 += i.id;
        }
        part2 += maxBlue * maxRed * maxGreen;
    }
    file.close();

    cout << "Part1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}