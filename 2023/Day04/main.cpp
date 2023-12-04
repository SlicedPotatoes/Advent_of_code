/*
  L'entrée représente une liste de cartes à gratter avec les numéros gagnants et les numéros grattés.

  Dans la partie 1, il faut calculer le gain total. Celui-ci se calcule comme suit:
  - Le premier match donne à la carte un score de 1.
  - Chaque match après le premier double le score.

  Dans la partie 2, on se rend compte que notre calcul de gain est erroné et que le prix à gagner est d'autres cartes à
  gratter:
  Si la carte X à Y numéros qui matchent avec les numéros gagnants, on gagne une copie de X + 1 jusqu'à X + Y.
  Exemple:
  Carte 1 à 3 numéros qui matchent: on récupère une copie des cartes --> 2, 3, 4
  L'objectif est de compter le nombre total de cartes que l'on a gagnées.
*/
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

struct Card
{
    // Les numéros gagnants et les numéros grattés de la carte
    vector<int> winnerNumbers;
    vector<int> scratchNumbers;
};

unordered_map<int, Card> listScratchCards;

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
        // Divise la ligne en deux parties distinctes : ID et les numéros gagnants/numéros grattés
        vector<string> splitedLine = splitString(line, ": ");

        // Récupère l'ID à partir de la sous-chaîne après le "Card"
        int id = stoi(splitedLine[0].substr(4));

        // Sépare les numéros gagnants et les numéros grattés
        vector<string> numbers = splitString(splitedLine[1], " | ");
        vector<string> winnerNumbersString = splitString(numbers[0], " ");
        vector<string> scratchNumbersString = splitString(numbers[1], " ");
        vector<int> winnerNumbers;
        vector<int> scratchNumbers;

        // Convertit les chaînes de caractères en entiers et les ajoute aux vecteurs correspondants
        for (string s : winnerNumbersString)
        {
            winnerNumbers.push_back(stoi(s));
        }
        for (string s : scratchNumbersString)
        {
            scratchNumbers.push_back(stoi(s));
        }

        // Ajoute la carte grattée à la liste des cartes grattées avec son ID
        listScratchCards[id] = {winnerNumbers, scratchNumbers};
    }
    file.close();

    return 0;
}

// Calcule le score d'une carte pour la partie 1
int cardScore(Card &c)
{
    int score = 0;

    // Parcourt les numéros grattés de la carte
    for (int scratch : c.scratchNumbers)
    {
        // Compare chaque numéro gratté avec les numéros gagnants
        for (int winner : c.winnerNumbers)
        {
            // Si c'est le premier match, attribue un score de 1, sinon double le score actuel
            if (scratch == winner)
            {
                score = score == 0 ? 1 : score * 2;
                break;
            }
        }
    }
    return score;
}

// Traite la partie 1
int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int sum = 0;

    // Parcourt la liste des cartes grattées et accumule les scores
    for (auto &it : listScratchCards)
    {
        sum += cardScore(it.second);
    }

    return sum;
}

// Obtient le nombre de correspondances entre les numéros grattés et gagnants pour une carte
int getNbWin(Card &c)
{
    int count = 0;

    // Parcourt les numéros grattés de la carte
    for (int scratch : c.scratchNumbers)
    {
        // Compare chaque numéro gratté avec les numéros gagnants et incrémente le compteur en cas de correspondance
        for (int winner : c.winnerNumbers)
        {
            if (scratch == winner)
            {
                count++;
                break;
            }
        }
    }

    return count;
}

// Mémoïse les résultats pour la partie 2 et obtient le score total d'une carte
unordered_map<int, int> cache;
int getScoreP2(int idCard)
{
    // Vérifie si le résultat pour cette carte est déjà enregistré dans le cache
    if (cache.find(idCard) != cache.end())
    {
        return cache[idCard];
    }

    int nbWin = getNbWin(listScratchCards[idCard]);

    int count = nbWin;

    // Récursivement, ajoute le score des cartes gagnées par la carte actuelle
    for (int i = 1; i <= nbWin; i++)
    {
        count += getScoreP2(idCard + i);
    }

    // Stocke le résultat dans le cache pour éviter de recalculer
    cache[idCard] = count;

    return count;
}

// Traite la partie 2
int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int count = listScratchCards.size();

    // Parcourt la liste des cartes grattées et ajoute le score total de chaque carte
    for (auto &it : listScratchCards)
    {
        count += getScoreP2(it.first);
    }

    return count;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processP1();
    int part2 = processP2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}