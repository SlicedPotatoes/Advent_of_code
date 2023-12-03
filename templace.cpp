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

int processPart1()
{
    int codeSize = 0;
    int memorySize = 0;

    for (string s : input)
    {
        codeSize += input.size();
    }

    cout << codeSize << " " << memorySize << '\n';
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    processPart1();

    return 0;
}