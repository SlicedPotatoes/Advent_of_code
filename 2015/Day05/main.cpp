/*
  L'entrée consiste en une liste de chaînes de caractères, avec pour objectif de déterminer si elles sont "gentilles" ou
  "méchantes".

  Pour la première partie, le processus permettant de déterminer une chaîne gentille est le suivant:
  - Elle doit contenir au moins 3 voyelles (aeiou).
  - Elle doit contenir au moins une lettre apparaissant deux fois consécutivement (aa, bb, cc, ...).
  - Elle ne doit pas contenir les chaînes suivantes : "ab", "cd", "pq", "xy".

  Pour la deuxième partie, le processus de détermination d'une chaîne gentille est le suivant:
  - Elle doit contenir une paire de lettres apparaissant au moins 2 fois dans la chaîne sans se chevaucher (e: "abab").
  - Elle doit contenir au moins une lettre se répétant avec exactement une lettre entre elles (e: "axa").

  Pour les deux parties, nous devons compter le nombre de chaînes "gentilles".
*/
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const string VOWELS = "aeiou";
const string BAD_STRINGS[5]{"ab", "cd", "pq", "xy"};

bool part1Process(string s)
{
    int countVowels = 0;
    bool twoConsecutiveEqualChar = false;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (VOWELS.find(s[i]) != string::npos)
        {
            countVowels++;
        }
        if (i != s.size() - 1)
        {
            if (s[i] == s[i + 1])
            {
                twoConsecutiveEqualChar = true;
            }
            for (string badString : BAD_STRINGS)
            {
                if (s[i] == badString[0] && s[i + 1] == badString[1])
                {
                    return false;
                }
            }
        }
    }
    return countVowels >= 3 && twoConsecutiveEqualChar;
}

bool part2Process(string s)
{
    bool findPair = false;
    for (size_t i = 0; i < s.size() - 1; i++)
    {
        for (size_t j = i + 2; j < s.size() - 1; j++)
        {
            if (s[i] == s[j] && s[i + 1] == s[j + 1])
            {
                findPair = true;
                break;
            }
        }
    }
    if (!findPair)
    {
        return false;
    }
    for (size_t i = 0; i < s.size() - 2; i++)
    {
        if (s[i] == s[i + 2])
        {
            return true;
        }
    }
    return false;
}

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
        if (part1Process(line))
        {
            part1++;
        }
        if (part2Process(line))
        {
            part2++;
        }
    }
    file.close();

    cout << "Part1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}