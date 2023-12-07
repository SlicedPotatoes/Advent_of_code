/*
  L'entrée représente une liste de mains de "Camel Cards" (une variante imaginaire du poker) et la mise associée.

  L'objectif des deux parties est de trier cette liste.

  Pour évaluer une main, on lui attribue un type, du plus fort au plus faible:
  - 5 Cartes identiques (Exemple : AAAAA)
  - 4 Cartes identiques (Exemple : AAAA9)
  - 3 Cartes identiques et 2 Cartes identiques (Exemple : AAA99)
  - 3 Cartes identiques seulement (Exemple : AAA98)
  - 2 Paires de 2 Cartes identiques (Exemple : AA998)
  - 2 Cartes identiques (Exemple : AA987)

  Le tri s'effectue en fonction du type. Cependant, si deux mains ont le même type, on compare les cartes une à une tant
  qu'elles sont les mêmes.
  Exemple: AAAA8 et AAAA9 ont le même type, mais AAAA9 sera considéré comme plus fort car 9 > 8.

  L'ordre des cartes, du plus fort au plus faible, est le suivant:
  A K Q J T 9 8 7 6 5 4 3 2

  Dans la partie 1, l'objectif est de trier la liste dans l'ordre croissant, puis de calculer le produit de la mise et
  du rang de la main. On doit retourner la somme des produits de chaque main.

  Exemple:

  Liste d'entrée:
  32T3K 765
  T55J5 684
  KK677 28
  KTJJT 220
  QQQJA 483

  Après le tri: 32T3K KTJJT KK677 T55J5 QQQJA
  Resultat: (765 * 1) + (220 * 2) + (28 * 3) + (684 * 4) + (483 * 5) = 6440

  Dans la partie 2, l'objectif est le même. Cependant, chaque carte J représente un joker qui vient compléter la main.
  Exemple : on a la main suivante --> AAAJ2 --> Le joker devient un A donc le type devient "4 Cartes Identiques".
  Pour compenser, l'ordre de force des cartes change pour le suivant :
  A K Q T 9 8 7 6 5 4 3 2 J
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const int CARD_IN_HAND = 5;
map<char, int> cardsOrder = {{'A', 13}, {'K', 12}, {'Q', 11}, {'J', 10}, {'T', 9}, {'9', 8}, {'8', 7},
                             {'7', 6},  {'6', 5},  {'5', 4},  {'4', 3},  {'3', 2}, {'2', 1}};

map<char, int> cardsOrderP2 = {{'A', 13}, {'K', 12}, {'Q', 11}, {'T', 10}, {'9', 9}, {'8', 8}, {'7', 7},
                               {'6', 6},  {'5', 5},  {'4', 4},  {'3', 3},  {'2', 2}, {'J', 1}};

// Structure représentant une main
struct Hand
{
    string card; // Cartes de la main
    int bid;     // Mise associée à la main
};

// Retourne le type de main pour une main donnée en paramètre
int getHandType(const Hand &h, bool isP2)
{
    // Map pour compter le nombre d'occurrences de chaque carte dans la main
    map<char, int> count;

    // Variable pour le comptage maximal d'occurrences
    int maxCount = 0;

    // Compteur spécifique pour le joker (cas Partie 2)
    int countJ = 0;

    // Parcours des cartes de la main
    for (char c : h.card)
    {
        // Vérification si la carte est un joker (cas Partie 2
        if (isP2)
        {
            if (c == 'J')
            {
                countJ++;
                continue;
            }
        }

        // Initialisation du compteur pour la carte si elle n'a pas encore été rencontrée
        if (count.find(c) == count.end())
        {
            count[c] = 0;
        }

        // Incrémentation du compteur pour la carte
        count[c]++;

        // Mise à jour du comptage maximal
        if (count[c] > maxCount)
        {
            maxCount = count[c];
        }
    }

    // Ajout du comptage des jokers s'il y en a (cas Partie 2)
    if (isP2)
    {
        maxCount += countJ;
    }

    // Détermination du type de main en fonction des règles du jeu
    if (maxCount == 2)
    {
        // 2 Cartes identiques
        if (count.size() == 4)
        {
            return 1;
        }
        // 2 Paires de 2 Cartes identiques
        else if (count.size() == 3)
        {
            return 2;
        }
    }
    else if (maxCount == 3)
    {
        // 3 Cartes identiques, le reste totalement différent
        if (count.size() == 3)
        {
            return 3;
        }
        // 3 Cartes identiques et 2 Cartes identiques
        else if (count.size() == 2)
        {
            return 4;
        }
    }
    else if (maxCount == 4)
    {
        // 4 Cartes identiques
        return 5;
    }
    else if (maxCount == 5)
    {
        // 5 Cartes identiques
        return 6;
    }

    return 0;
}

// Opérateur de comparaison pour la Partie 1
struct comparator_p1
{
    bool operator()(const Hand &a, const Hand &b)
    {
        // Récupération du type de main pour les mains A et B
        int typeA = getHandType(a, false);
        int typeB = getHandType(b, false);

        // Si les types de main sont égaux, comparer les cartes une à une
        if (typeA == typeB)
        {
            for (int i = 0; i < CARD_IN_HAND; i++)
            {
                // Récupération de la valeur de la carte selon l'ordre défini
                int valueCardA = cardsOrder[a.card[i]];
                int valueCardB = cardsOrder[b.card[i]];

                // Si les valeurs sont égales, passer à la carte suivante
                if (valueCardA == valueCardB)
                {
                    continue;
                }

                // Retourner le résultat de la comparaison des valeurs des cartes
                return valueCardA < valueCardB;
            }
        }

        // Retourner le résultat de la comparaison des types de main
        return typeA < typeB;
    }
};

// Opérateur de comparaison pour la Partie 2
struct comparator_p2
{
    bool operator()(const Hand &a, const Hand &b)
    {
        // Récupération du type de main pour les mains A et B (Partie 2)
        int typeA = getHandType(a, true);
        int typeB = getHandType(b, true);

        // Si les types de main sont égaux, comparer les cartes une à une
        if (typeA == typeB)
        {
            for (int i = 0; i < CARD_IN_HAND; i++)
            {
                // Récupération de la valeur de la carte selon l'ordre défini
                int valueCardA = cardsOrderP2[a.card[i]];
                int valueCardB = cardsOrderP2[b.card[i]];

                // Si les valeurs sont égales, passer à la carte suivante
                if (valueCardA == valueCardB)
                {
                    continue;
                }

                // Retourner le résultat de la comparaison des valeurs des cartes
                return valueCardA < valueCardB;
            }
        }

        // Retourner le résultat de la comparaison des types de main
        return typeA < typeB;
    }
};

vector<Hand> hands;

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
        hands.push_back({splitedInput[0], stoi(splitedInput[1])});
    }
    file.close();
    return 0;
}

int process(bool isP2)
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Variable pour stocker le résultat final
    int result = 0;

    // Tri des mains en fonction de la partie traitée (avec ou sans jokers)
    if (isP2)
    {
        sort(hands.begin(), hands.end(), comparator_p2());
    }
    else
    {
        sort(hands.begin(), hands.end(), comparator_p1());
    }

    // Calcul du résultat en fonction des mises et du rang des mains
    for (int i = 0; i < hands.size(); i++)
    {
        result += hands[i].bid * (i + 1);
    }

    // Retourne le résultat final
    return result;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = process(false);
    int part2 = process(true);

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}