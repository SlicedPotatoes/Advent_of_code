/*
  L'entrée représente une liste de temps et une distance qui correspond au record pour cette course.

  Nous participons à une course de bateaux jouets, dont le fonctionnement est le suivant :
  Ils ont un bouton, chaque milliseconde que l'on reste appuyé dessus augmente leur vitesse d'1 millimètre/s.

  Prenons comme exemple une course qui dure 5 ms. Nous avons plusieurs possibilités :
  - On reste appuyer 0ms --> il a une vitesse de 0mm/s pour les 5ms suivantes --> distance parcourue: 0 mm.
  - On reste appuyer 1ms --> il a une vitesse de 1mm/s pour les 4ms suivantes --> distance parcourue: 4 mm.
  - On reste appuyer 2ms --> il a une vitesse de 2mm/s pour les 3ms suivantes --> distance parcourue: 6 mm.
  - On reste appuyer 3ms --> il a une vitesse de 3mm/s pour les 2ms suivantes --> distance parcourue: 6 mm.
  - On reste appuyer 4ms --> il a une vitesse de 4mm/s pour les 1ms suivantes --> distance parcourue: 4 mm.
  - On reste appuyer 5ms --> il a une vitesse de 5mm/s pour les 0ms suivantes --> distance parcourue: 0 mm.

  Pour la partie 1, l'objectif est de trouver, pour chaque course, le nombre de possibilités qui nous font battre le
  record.
  La réponse attendue est le produit du nombre de possibilités pour chaque course.

  Dans la partie 2, nous nous rendons compte que nous avons mal lu l'entrée et qu'il y a une seule course avec la
  concaténation des temps et de la distance pour le record.

  Par exemple, pour:
  Time:      7  15   30
  Distance:  9  40  200

  devient :

  Time:      71530
  Distance:  940200

  Dans ces conditions, combien avons-nous de possibilités pour battre le record ?
*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

// Structure représentant une course de bateau jouet
struct Race
{
    int time;     // Temps de la course en millisecondes
    int distance; // Distance correspondant au record en millimètres
};

vector<Race> races;

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
    vector<int> t;
    vector<int> d;
    while (getline(file, line))
    {
        vector<string> splitedInput = splitString(splitString(line, ":")[1], " ");
        bool time = t.size() == 0;
        for (string s : splitedInput)
        {
            if (time)
            {
                t.push_back(stoi(s));
            }
            else
            {
                d.push_back(stoi(s));
            }
        }
    }

    file.close();

    for (int i = 0; i < t.size(); i++)
    {
        races.push_back({t[i], d[i]});
    }

    return 0;
}

int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Variable pour stocker le produit final
    int product = 1;

    // Parcours de chaque course dans le vecteur races
    for (Race &r : races)
    {
        // Variable pour compter le nombre de façons de battre le record pour la course actuelle
        int countWin = 0;

        // Itération sur chaque possible temps d'appui sur le bouton
        for (int timePush = 0; timePush < r.time; timePush++)
        {
            // Calcul du temps de mouvement restant après l'appui sur le bouton
            int timeMove = r.time - timePush;

            // Calcul de la distance parcourue avec la vitesse résultante
            int d = timeMove * timePush;

            // Vérification si la distance parcourue dépasse le record
            if (d > r.distance)
            {
                countWin++;
            }

            // Si la condition n'est plus satisfaite et countWin n'est pas 0, on sort de la boucle
            else if (countWin != 0)
            {
                break;
            }
        }
        // Multiplication du produit partiel par le nombre de façons de battre le record pour la course actuelle
        product *= countWin;
    }
    // Retour du produit final
    return product;
}

int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Variables pour stocker les chaînes de caractères représentant le temps et la distance
    string timeS = "";
    string distanceS = "";

    // Concaténation des temps et des distances pour chaque course
    for (Race &r : races)
    {
        timeS += to_string(r.time);
        distanceS += to_string(r.distance);
    }

    // Conversion des chaînes en nombres long long
    long long time = stoll(timeS);
    long long distance = stoll(distanceS);

    // Variable pour compter le nombre de façons de battre le record
    int countWin = 0;

    // Itération sur chaque possible temps d'appui sur le bouton
    for (long long timePush = 0; timePush < time; timePush++)
    {
        // Calcul du temps de mouvement restant après l'appui sur le bouton
        long long timeMove = time - timePush;

        // Calcul de la distance parcourue avec la vitesse résultante
        long long d = timeMove * timePush;

        // Vérification si la distance parcourue dépasse le record combiné de toutes les courses
        if (d > distance)
        {
            countWin++;
        }

        // Si la condition n'est plus satisfaite et countWin n'est pas 0, on sort de la boucle
        else if (countWin != 0)
        {
            break;
        }
    }
    // Retour du nombre de façons de battre le record
    return countWin;
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