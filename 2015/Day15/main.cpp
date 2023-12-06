/*
  L'objectif est de fabriquer le meilleur cookie possible !

  L'entrée représente une liste d'ingrédients et leur impact sur les différentes caractéristiques du cookie:
  - Capacity
  - Durability
  - Flavor
  - Texture
  - Calories

  Pour fabriquer un cookie, nous devons choisir un total de 100 ingrédients parmi la liste.

  Pour calculer le score d'un cookie, on fait comme suit:
  - On additionne chacune des propriétés
  - On multiplie les propriétés entre elles
  - Si une propriété est négative, elle devient zéro (donc le résultat du produit sera forcément 0)

  Exemple:

  Liste d'ingrédients:
  Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
  Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3

  Nous avons choisi de mettre 44 Butterscotch et 56 Cinnamon, l'impact sur les caractéristiques:
  - Capacity = 44 * -1 + 56 * 2 = 68
  - Durability = 44 * -2 + 56 * 3 = 80
  - Flavor = 44 * 6 + 56 * -2 = 152
  - Texture = 44 * 3 + 56 * -1 = 76

  Score = 68 * 80 * 152 * 76 = 62 842 880

  Dans la partie 1, le but est de trouver le meilleur cookie possible avec la liste des ingrédients et de retourner son
  score.

  Dans la partie 2, le but est de trouver le meilleur cookie possible avec un nombre de calories égal à 500 et de
  retourner son score.

*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const int MAX_INGREDIENTS = 100;
const int MAX_CALORIES = 500;

// Structure représentant un ingrédient
struct Ingredient
{
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};

// Structure représentant un cookie
struct Cookie
{
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
};

vector<Ingredient> listIngredients;
vector<vector<int>> allPossibleCookie;

// Génère toutes les combinaisons possibles de valeurs pour créer un cookie
void genCombinations(vector<int> &values, int remaining, int currentIndex)
{
    // Cas de base : si toutes les valeurs ont été attribuées
    if (remaining == 0)
    {
        vector<int> cookie;
        // Copie des valeurs dans le vecteur cookie
        for (int element : values)
        {
            cookie.push_back(element);
        }
        // Ajout du cookie à la liste de toutes les combinaisons possibles
        allPossibleCookie.push_back(cookie);
        return;
    }

    // Génération des combinaisons récursivement
    for (int i = currentIndex; i < values.size(); i++)
    {
        if (remaining > 0)
        {
            // Affectation des valeurs de 0 à remaining à l'élément en cours
            for (int j = 0; j <= remaining; j++)
            {
                values[i] = j;
                // Appel récursif pour les combinaisons restantes
                genCombinations(values, remaining - j, i + 1);
                // Réinitialisation de la valeur à 0 pour le prochain tour de boucle
                values[i] = 0;
            }
        }
    }
}

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

        // Extraction des valeurs numériques et ajout à la liste des ingrédients
        int capacity = stoi(splitedInput[2].replace(splitedInput[2].find(','), 1, ""));
        int durability = stoi(splitedInput[4].replace(splitedInput[4].find(','), 1, ""));
        int flavor = stoi(splitedInput[6].replace(splitedInput[6].find(','), 1, ""));
        int texture = stoi(splitedInput[8].replace(splitedInput[8].find(','), 1, ""));
        int calories = stoi(splitedInput[10]);

        listIngredients.push_back({capacity, durability, flavor, texture, calories});
    }
    file.close();

    // Génération de toutes les combinaisons possibles
    vector<int> values(listIngredients.size(), 0);
    genCombinations(values, MAX_INGREDIENTS, 0);

    return 0;
}

// Créer un cookie à partir d'une liste d'ingrédients et de quantités
Cookie getCookie(vector<int> &list)
{
    // Initialisation des propriétés du cookie
    int capacity = 0;
    int durability = 0;
    int flavor = 0;
    int texture = 0;
    int calories = 0;

    // Calcul des propriétés du cookie en fonction des ingrédients et de leurs quantités
    for (int i = 0; i < listIngredients.size(); i++)
    {
        capacity += listIngredients[i].capacity * list[i];
        durability += listIngredients[i].durability * list[i];
        flavor += listIngredients[i].flavor * list[i];
        texture += listIngredients[i].texture * list[i];
        calories += listIngredients[i].calories * list[i];
    }

    // Création et retour du cookie
    return {capacity, durability, flavor, texture, calories};
}

// Calculer le score d'un cookie
int calculScore(Cookie &c)
{
    // Vérification des propriétés du cookie, si l'une d'entre elles est négative, le score est 0
    if (c.capacity < 0 || c.durability < 0 || c.flavor < 0 || c.texture < 0)
    {
        return 0;
    }

    // Calcul du score en multipliant les propriétés du cookie
    return c.capacity * c.durability * c.flavor * c.texture;
}

// Calculer le score d'un cookie à partir d'une liste de quantités
int calculScore(vector<int> &list)
{
    // Obtention du cookie à partir de la liste de quantités
    Cookie c = getCookie(list);

    // Calcul du score à partir du cookie obtenu
    return calculScore(c);
}

// Fonction pour traiter la partie 1 et trouver le meilleur score parmi toutes les combinaisons de cookies
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation du score maximum
    int max = 0;

    // Parcours de toutes les combinaisons de cookies
    for (vector<int> &cookie : allPossibleCookie)
    {
        // Calcul du score pour le cookie actuel
        int score = calculScore(cookie);

        // Mise à jour du score maximum si le score actuel est plus élevé
        if (score > max)
        {
            max = score;
        }
    }

    // Retour du score maximum obtenu
    return max;
}

// Fonction pour traiter la partie 2 et trouver le meilleur score parmi toutes les combinaisons de cookies avec un
// nombre de calories fixe
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialisation du score maximum
    int max = 0;

    // Parcours de toutes les combinaisons de cookies
    for (vector<int> &cookie : allPossibleCookie)
    {
        // Obtention du cookie à partir de la liste d'ingrédients et de quantités
        Cookie c = getCookie(cookie);

        // Vérification du nombre de calories du cookie
        if (c.calories == MAX_CALORIES)
        {
            // Calcul du score pour le cookie actuel
            int score = calculScore(c);

            // Mise à jour du score maximum si le score actuel est plus élevé
            if (score > max)
            {
                max = score;
            }
        }
    }

    // Retour du score maximum obtenu
    return max;
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