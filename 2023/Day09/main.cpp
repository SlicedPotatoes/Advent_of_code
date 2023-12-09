/*

  L'entrée représente une liste de séquences.

  Dans la première partie, l'objectif est de prédire le nombre suivant dans la séquence. Le résultat attendu est la
  somme des nombres prédits.

  Dans la deuxième partie, l'objectif est de prédire le nombre précédant le premier nombre de la séquence. Le résultat
  attendu est la somme des nombres prédits.

  Pour prédire un nombre dans une séquence, procédez comme suit:
  - Pour chaque nombre de l'index + 1 jusqu'à n, calculez la différence avec son voisin de gauche (index - 1).
  - Créez une nouvelle séquence avec les différences.
  - Répétez tant que la dernière ligne n'est pas remplie de zéros.

  Exemple:
  10 13 16 21 30 45
    3  3  5  9  15
      0  2  4  6
        2  2  2
          0  0

  Pour prédire le nombre suivant, ajoutez un 0 à la dernière ligne. Pour les lignes suivantes, ajoutez la somme du
  dernier numéro de la ligne avec le dernier numéro de la ligne suivante.
  Pour prédire le nombre précédent, faites la même chose, mais utilisez la différence entre le premier numéro de la
  ligne et le premier numéro de la ligne suivante.

  5 10 13 16 21 30 45  68
   5  3  3  5  9  15  23
    -2  0  2  4  6  8
       2  2  2  2  2
         0  0  0  0

*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

vector<vector<int>> inputs;

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
        vector<int> vectorInputInt;
        for (string s : splitedInput)
        {
            vectorInputInt.push_back(stoi(s));
        }
        inputs.push_back(vectorInputInt);
    }
    file.close();

    return 0;
}

// Retourne un vecteur contenant deux nombres. Le premier nombre représente le nombre suivant prédit tandis que le
// deuxième nombre représente le nombre précédant le premier de la séquence.

vector<int> processSequence(vector<int> &sequence)
{
    bool isEnd = false;
    vector<vector<int>> sequences = {sequence};

    // Générer des séquences dérivées jusqu'à ce que la dernière ligne soit remplie de zéros.
    while (!isEnd)
    {
        isEnd = true;
        vector<int> _sequence = sequences.back();
        vector<int> next;

        // Calculer les différences entre les éléments consécutifs et détecter si la séquence est terminée.
        for (int i = 1; i < _sequence.size(); i++)
        {
            int diff = _sequence[i] - _sequence[i - 1];
            if (diff != 0)
            {
                isEnd = false;
            }
            next.push_back(diff);
        }

        sequences.push_back(next);
    }

    // Ajuster les séquences pour prédire le nombre suivant et le nombre précédant.
    for (int i = sequences.size() - 1; i >= 0; i--)
    {
        if (i == sequences.size() - 1)
        {
            sequences[i].insert(sequences[i].begin(), 0);
            sequences[i].push_back(0);
            continue;
        }
        sequences[i].insert(sequences[i].begin(), sequences[i].front() - sequences[i + 1].front());
        sequences[i].push_back(sequences[i].back() + sequences[i + 1].back());
    }

    // Récupérer les résultats finaux.
    vector<int> result = {sequences[0].back(), sequences[0].front()};
    return result;
}

vector<int> process()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialiser le résultat avec des zéros.
    vector<int> result = {0, 0};

    // Parcours de chaque séquence d'entrée.
    for (vector<int> oneInput : inputs)
    {
        // Appel de la fonction processSequence pour chaque séquence.
        vector<int> _result = processSequence(oneInput);

        // Faire la somme des résultats.
        result[0] += _result[0];
        result[1] += _result[1];
    }

    return result;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    vector<int> result = process();

    cout << "\nPart1: " << result[0];
    cout << "\nPart2: " << result[1];

    return 0;
}