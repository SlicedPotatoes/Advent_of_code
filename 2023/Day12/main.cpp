#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const char UNKNOWN_SPRING = '?';
const char BROKEN_SPRING = '#';
const char FUNCTIONAL_SPRING = '.';

const vector<char> PROCESS_HELPER = {BROKEN_SPRING, FUNCTIONAL_SPRING};

struct Line
{
    string s;
    vector<int> controls;
};

vector<Line> inputs;

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
        vector<string> splitedInput = splitString(line, " ");
        vector<string> stringControls = splitString(splitedInput[1], ",");
        vector<int> controls;

        for (string s : stringControls)
        {
            controls.push_back(stoi(s));
        }

        inputs.push_back({splitedInput[0], controls});
    }
    file.close();

    return 0;
}

unordered_map<string, long long> cache;
long long getNbCombinations(string s, vector<int> &d, size_t indexChar, size_t indexBlock, int current)
{
    string key = to_string(indexChar) + '_' + to_string(indexBlock) + '_' + to_string(current);

    if (cache.find(key) != cache.end())
    {
        return cache[key];
    }

    if (indexChar == s.size())
    {
        if (indexBlock == d.size() && current == 0)
        {
            return 1;
        }
        else if (indexBlock == d.size() - 1 && current == d[indexBlock])
        {
            return 1;
        }
        return 0;
    }

    long long count = 0;
    for (char c : PROCESS_HELPER)
    {
        if (s[indexChar] == c || s[indexChar] == UNKNOWN_SPRING)
        {
            if (c == FUNCTIONAL_SPRING && current == 0)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock, 0);
            }
            else if (c == FUNCTIONAL_SPRING && current > 0 && indexBlock < d.size() && d[indexBlock] == current)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock + 1, 0);
            }
            else if (c == BROKEN_SPRING)
            {
                count += getNbCombinations(s, d, indexChar + 1, indexBlock, current + 1);
            }
        }
    }
    cache[key] = count;
    return count;
}

long long processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    long long count = 0;

    for (Line l : inputs)
    {
        count += getNbCombinations(l.s, l.controls, 0, 0, 0);
        cache.clear();
    }

    return count;
}

long long processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    long long count = 0;

    for (Line l : inputs)
    {
        string s = l.s;
        vector<int> vec;
        for (int i = 0; i < 4; i++)
        {
            s += UNKNOWN_SPRING + l.s;
        }
        for (int i = 0; i < 5; i++)
        {
            for (int element : l.controls)
            {
                vec.push_back(element);
            }
        }

        count += getNbCombinations(s, vec, 0, 0, 0);
        cache.clear();
    }

    return count;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    long long part1 = processPart1();
    long long part2 = processPart2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}