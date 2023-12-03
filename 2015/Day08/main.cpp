/*
    L'entrée représente une liste de chaînes de caractères, comme on les représenterait dans du code, c'est-à-dire avec
    des '"' au début et des caractères d'échappement '\'.

    Pour la partie 1, l'objectif est de calculer la différence de taille entre la chaîne encodée pour être écrite dans
    du code et la taille en mémoire.

    Pour la partie 2, l'objectif est le même que pour la partie 1, sauf que l'on doit encoder la chaîne en lui ajoutant
    des '"' et des caractères d'espacement.

    Le calcul de la taille se fait au nombre de caractères.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

vector<string> input;

int parseInput()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    string filename = "input.txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier " << filename << '\n';
        return 1;
    }
    string line;
    while (getline(file, line))
    {
        input.push_back(line);
    }
    file.close();
    return 0;
}

// Fonction pour obtenir la taille en mémoire d'une chaîne donnée.
int getMemorySize(string &s)
{
    int n = 0;
    for (size_t i = 1; i < s.size() - 1; i++)
    {
        if (s[i] == '\\')
        {
            char nextChar = s[i + 1];
            if (nextChar == '\\' || nextChar == '\"')
            {
                i++;
            }
            else if (nextChar == 'x')
            {
                i += 3;
            }
        }
        n++;
    }
    return n;
}
// Fonction pour obtenir la taille de la chaîne encodée.
int sizeStringEncoded(string &s)
{
    string newString = "\"";

    for (char c : s)
    {
        if (c == '\\')
        {
            newString += "\\\\";
        }
        else if (c == '"')
        {
            newString += "\\\"";
        }
        else
        {
            newString += c;
        }
    }

    newString += '"';

    return newString.size();
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int codeSize = 0;
    int memorySize = 0;

    for (string s : input)
    {
        codeSize += s.size();
        memorySize += getMemorySize(s);
    }

    return codeSize - memorySize;
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int codeSize = 0;
    int memorySize = 0;

    for (string s : input)
    {
        codeSize += sizeStringEncoded(s);
        memorySize += s.size();
    }

    return codeSize - memorySize;
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