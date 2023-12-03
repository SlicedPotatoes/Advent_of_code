/*
  L'entrée représente une suite de nombres.

  L'objectif est d'appliquer la règle "count and say" (compter et dire):
  Exemple:
  - 1 --> 11 (un 1)
  - 11 --> 21 (deux 1)
  - 21 --> 1211 (un 2 un 1)
  - 1211 --> 111211 (un 1 un 2 deux 1)
  - ect

  Partie 1 : Retourner la taille de la séquence après avoir appliqué 40 fois la règle décrite ci-dessus à notre entrée.

  Partie 2 : Même objectif que la partie 1, sauf qu'il faut appliquer la règle 50 fois.
*/
#include <fstream>
#include <iostream>
#include <string>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

const int NUMBER_OF_ITERATIONS_PART_1 = 40;
const int NUMBER_OF_ITERATIONS_PART_2 = 50;

string input;

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
    getline(file, input);
    file.close();
    return 0;
}

string nextSequence(string s)
{
    string newString = "";

    for (int i = 0; i < s.size(); i++)
    {
        int index = i + 1;
        while (index < s.size() && s[index] == s[i])
        {
            index++;
        }
        newString += to_string(index - i) + s[i];
        i = index - 1;
    }

    return newString;
}

int process(int n)
{
    MEASURE_FUNCTION_EXECUTION_TIME
    string s = input;

    for (int i = 0; i < n; i++)
    {
        s = nextSequence(s);
    }

    return s.size();
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = process(NUMBER_OF_ITERATIONS_PART_1);
    int part2 = process(NUMBER_OF_ITERATIONS_PART_2);

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}