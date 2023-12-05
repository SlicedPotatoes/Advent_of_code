/*
  L'entrée représente une liste de rennes et leurs caractéristiques:
  - Leurs vitesses
  - Le temps qu'ils peuvent maintenir cette vitesse
  - Le temps de repos avant de pouvoir repartir

  L'objectif pour la partie 1 est de calculer, pour chaque renne, la distance maximale parcourue et de renvoyer celle la
  plus élevée.

  L'objectif pour la partie 2 est le suivant : on décide d'attribuer un score à chaque renne :
  - Un renne étant premier à un instant T voit son score augmenté de 1.
  - Si plusieurs rennes sont premiers, chacun voit son score augmenté de 1.
  Il faut renvoyer le score maximal.

  Pour les 2 parties, la simulation s'arrête après 2503 secondes.
*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const int MAX_TIME = 2503;

struct Renne
{
    int speed;         // Vitesse du renne
    int timeFullSpeed; // Temps pendant lequel le renne peut maintenir sa vitesse
    int timeSleep;     // Temps de repos nécessaire avant que le renne puisse repartir

    int distance = 0; // Distance parcourue par le renne (initialisée à 0)
    int score = 0;    // Score attribué au renne (initialisé à 0)
};

vector<Renne> listRennes;
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
        int speed = stoi(splitedInput[3]);
        int timeFullSpeed = stoi(splitedInput[6]);
        int timeSleep = stoi(splitedInput[13]);

        listRennes.push_back({speed, timeFullSpeed, timeSleep});
    }
    file.close();

    return 0;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int maxValue = 0;

    // Parcours des instants de temps jusqu'à MAX_TIME
    for (int i = 0; i < MAX_TIME; i++)
    {
        // Parcours de la liste des rennes
        for (Renne &r : listRennes)
        {
            // Vérification si le renne est en phase de course à cette instant de temps
            if (i % (r.timeFullSpeed + r.timeSleep) < r.timeFullSpeed)
            {
                r.distance += r.speed;                // Mise à jour de la distance parcourue par le renne
                maxValue = max(maxValue, r.distance); // Mise à jour de la distance maximale parcourue
            }
        }
    }
    return maxValue; // Retour de la distance maximale parcourue par un renne
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int maxValue = 0;

    // Réinitialisation des distances parcourues par tous les rennes
    for (Renne &r : listRennes)
    {
        r.distance = 0;
    }

    // Parcours des instants de temps jusqu'à MAX_TIME
    for (int i = 0; i < MAX_TIME; i++)
    {
        int maxDistance = 0;
        vector<Renne *> first; // Liste des rennes en tête

        // Parcours de la liste des rennes
        for (Renne &r : listRennes)
        {
            // Vérification si le renne est en phase de course à cet instant de temps
            if (i % (r.timeFullSpeed + r.timeSleep) < r.timeFullSpeed)
            {
                r.distance += r.speed; // Mise à jour de la distance parcourue par le renne
            }

            // Gestion des rennes à la même distance maximale
            if (r.distance == maxDistance)
            {
                first.push_back(&r);
            }
            // Gestion d'un renne dépassant la distance maximale actuelle
            else if (r.distance > maxDistance)
            {
                maxDistance = r.distance;
                first.clear();
                first.push_back(&r);
            }
        }

        // Attribution des scores aux rennes en tête et mise à jour du score maximal
        for (Renne *r : first)
        {
            r->score++;
            maxValue = max(maxValue, r->score);
        }
    }
    return maxValue; // Retour du score maximal attribué à un renne
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