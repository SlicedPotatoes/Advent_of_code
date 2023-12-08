#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

int input;

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
    getline(file, line);
    input = stoi(line);
    file.close();
    return 0;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int element = 150;

    int i = 10;

    while (element > 0)
    {
        if (element % i == 0)
        {
            element -= i;
        }
        cout << "I: " << i << " Element: " << element << '\n';
        i += 10;
    }

    return (i - 10) / 10;
}

/*int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int element = 150;

    vector<int> value;

    for (int i = 10; i < element; i += 10)
    {
        if (element % i == 0)
        {
            value.push_back(i);
        }
    }

    int a = 0;

    for (int e : value)
    {
        cout << e << ' ';
        a += e;
        if (a >= element)
        {
            break;
        }
    }

    return a / 10;
}*/

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();

    cout << "\nPart1: " << part1 << '\n';

    return 0;
}