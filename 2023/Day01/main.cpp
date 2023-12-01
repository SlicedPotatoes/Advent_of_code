/*
  Dans l'entrée, nous avons plusieurs chaînes de caractères.

  Pour la partie 1, pour chaque ligne, il est nécessaire de concaténer le premier et le dernier chiffre, puis de
  retourner la somme de ces concaténations.

  Pour la partie 2, le processus est similaire, à la différence près qu'il faut
  tenir compte du fait que certaines valeurs sont écrites en lettres (one, two, three, ...).
*/

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const map<string, char> MAP_NUMBER{{"one", '1'}, {"two", '2'},   {"three", '3'}, {"four", '4'}, {"five", '5'},
                                   {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};

char indexOfP1(string s, bool lastIndexOf)
{
    size_t startI = lastIndexOf ? s.size() - 1 : 0;
    size_t endI = lastIndexOf ? -1 : s.size();
    size_t incI = lastIndexOf ? -1 : 1;

    for (size_t i = startI; i != endI; i += incI)
    {
        if (isdigit(s[i]))
        {
            return s[i];
        }
    }

    return 'f';
}

char isNumberLetter(string s, size_t index)
{
    for (const auto &kv : MAP_NUMBER)
    {
        string n = kv.first;
        bool valid = true;
        for (size_t i = 0; i < n.size(); i++)
        {
            if (s[index + i] != n[i])
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            return kv.second;
        }
    }
    return 'f';
}

char indexOfP2(string s, bool lastIndexOf)
{
    size_t startI = lastIndexOf ? s.size() - 1 : 0;
    size_t endI = lastIndexOf ? -1 : s.size();
    size_t incI = lastIndexOf ? -1 : 1;

    for (size_t i = startI; i != endI; i += incI)
    {
        char indexLetterNumber = isNumberLetter(s, i);
        if (indexLetterNumber != 'f')
        {
            return indexLetterNumber;
        }
        if (isdigit(s[i]))
        {
            return s[i];
        }
    }

    return 'f';
}

int main()
{
    string filename = "input.txt";
    ifstream file(filename);

    int sumP1 = 0;
    int sumP2 = 0;

    if (!file.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
        return 1;
    }
    string line;
    while (getline(file, line))
    {
        vector<char> charVectorP1;
        charVectorP1.push_back(indexOfP1(line, false));
        charVectorP1.push_back(indexOfP1(line, true));
        string calibrationValueP1(charVectorP1.begin(), charVectorP1.end());

        vector<char> charVectorP2;
        charVectorP2.push_back(indexOfP2(line, false));
        charVectorP2.push_back(indexOfP2(line, true));

        string calibrationValueP2(charVectorP2.begin(), charVectorP2.end());

        sumP1 += stoi(calibrationValueP1);
        sumP2 += stoi(calibrationValueP2);
    }
    file.close();

    cout << "Part1: " << sumP1 << '\n';
    cout << "Part2: " << sumP2 << '\n';

    return 0;
}