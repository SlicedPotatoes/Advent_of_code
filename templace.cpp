#include <fstream>
#include <iostream>
#include <string>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

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
    }
    file.close();
    return 0;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    return 0;
}