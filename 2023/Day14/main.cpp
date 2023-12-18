/*
  L'entrée représente une carte avec des cases vides, des rochers qui peuvent être déplacés, et des obstacles qui ne
  bougent pas.

  L'objectif pour la partie 1 est de simuler la gravité des rochers vers le nord.

  L'objectif pour la partie 2 est de simuler 1000000000 cycles où chaque cycle simule la gravité vers le nord, puis
  l'ouest, le sud et l'est.

  La réponse à retourner pour les 2 parties est la charge totale qui se calcule de la manière suivante:
  La quantité de charge causée par un rocher est égale au nombre de rangées depuis le rocher jusqu'au bord sud
  exemple:

  OOOO.#.O.. 10
  OO..#....#  9
  OO..O##..O  8
  O..#.OO...  7
  ........#.  6
  ..#....#.#  5
  ..O..#.O.O  4
  ..O.......  3
  #....###..  2
  #....#....  1

  Dans cette situation, la charge totale est 136.
  Les rochers (O) sur la première ligne ont une valeur de 10, sur la 2e 9 etc.

  La partie 2 ayant un nombre absurde de simulations, la technique que j'ai utilisée est de trouver un motif répétitif
  dans les cycles, me permettant d'arrêter l'itération et de simplement calculer l'indice par rapport aux grilles
  précédemment sauvegardée pour trouver l'état final.
*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

vector<string> input;

static char MOVE_ROCK = 'O';
static char EMPTY = '.';
int PART_2_LIMIT = 1000000000;

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
        input.push_back(line);
    }
    file.close();

    return 0;
}

// Cette fonction simule la gravité des rochers dans une ligne spécifique (horizontalement ou verticalement).
// Les paramètres x et y représentent les coordonnées du point de départ, et n représente la direction dans laquelle
// simuler la gravité. Lorsque n == 0, on simule la gravité vers le nord ; n == 1, vers l'ouest ; n == 2, vers le sud ;
// n == 3, vers l'est.
void gravityLine(int x, int y, int n)
{
    // Vérifier si un rocher se trouve derrière (par rapport à la direction) de celui que l'on va simuler.
    bool other = false;
    switch (n)
    {
    case 0:
        other = y + 1 < input.size() && input[y + 1][x] == MOVE_ROCK;
        break;
    case 1:
        other = x + 1 < input[0].size() && input[y][x + 1] == MOVE_ROCK;
        break;
    case 2:
        other = y - 1 >= 0 && input[y - 1][x] == MOVE_ROCK;
        break;
    case 3:
        other = x - 1 >= 0 && input[y][x - 1] == MOVE_ROCK;
        break;
    }

    int _x = x;
    int _y = y;

    // Détermine les increments et limites pour déplacer les rochers dans la direction spécifiée.
    int xInc = n == 1 ? -1 : n == 3 ? 1 : 0;
    int yInc = n == 0 ? -1 : n == 2 ? 1 : 0;
    int xEnd = n == 1 ? 0 : n == 3 ? input[0].size() - 1 : x;
    int yEnd = n == 0 ? 0 : n == 2 ? input.size() - 1 : y;

    // Déplace les rochers jusqu'à ce qu'ils atteignent une case non vide ou les limites définies.
    while ((_y != yEnd || _x != xEnd) && input[_y + yInc][_x + xInc] == EMPTY)
    {
        input[_y][_x] = EMPTY;
        _y += yInc;
        _x += xInc;
        input[_y][_x] = MOVE_ROCK;
    }

    // Répète le processus pour simuler la gravité de manière récursive.
    if (other)
    {
        gravityLine(x - xInc, y - yInc, n);
    }
}

// Cette fonction simule la gravité des rochers sur l'ensemble de la grille dans une direction spécifique.
// Le paramètre n représente la direction dans laquelle simuler la gravité (0 pour le nord, 1 pour l'ouest, 2 pour le
// sud, 3 pour l'est).
void gravityGrid(int n)
{
    for (int y = 0; y < input.size(); y++)
    {
        for (int x = 0; x < input[0].size(); x++)
        {
            // Si la case contient un rocher, simule la gravité pour cette position
            if (input[y][x] == MOVE_ROCK)
            {
                gravityLine(x, y, n);
            }
        }
    }
}

// Cette fonction calcule la charge totale en parcourant la grille de rochers donnée.
// La charge causée par un rocher est égale au nombre de rangées depuis le rocher jusqu'au bord sud.
int totalLoad(vector<string> s)
{
    int sum = 0;

    for (int y = 0; y < s.size(); y++)
    {
        for (int x = 0; x < s[0].size(); x++)
        {
            // Si la case contient un rocher, ajoute la charge calculée à la somme totale.
            if (s[y][x] == MOVE_ROCK)
            {
                sum += s.size() - y;
            }
        }
    }

    return sum;
}

// Fonction pour traiter la Partie 1 du problème
int processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Effectue une copie de la grille d'entrée.
    vector<string> copy = input;

    // Simule la gravité des rochers vers le nord.
    gravityGrid(0);

    // Calcule la charge totale après la simulation.
    int answer = totalLoad(input);

    // Restaure la grille d'entrée à partir de la copie.
    input = copy;

    return answer;
}

// Fonction pour traiter la Partie 2 du problème
int processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Initialise une copie de la grille pour stocker les états précédents.
    vector<vector<string>> copyGrid;

    copyGrid.push_back(input);
    int indexCycle = -1;

    // Effectue la simulation de gravité dans les quatre directions pour un certain nombre de cycles.
    for (int i = 0; i < PART_2_LIMIT; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gravityGrid(j);
        }

        bool isSame = true;

        // Vérifie si l'état actuel a déjà été rencontré dans les cycles précédents.
        for (int indexCopy = 0; indexCopy < copyGrid.size(); indexCopy++)
        {
            isSame = true;
            for (int indexString = 0; indexString < input.size(); indexString++)
            {
                if (input[indexString] != copyGrid[indexCopy][indexString])
                {
                    isSame = false;
                }
            }
            if (isSame)
            {
                indexCycle = indexCopy;
                break;
            }
        }

        // Si l'état actuel a déjà été rencontré, sort de la boucle.
        if (isSame)
        {
            break;
        }

        // Ajoute l'état actuel à la liste des états précédents.
        copyGrid.push_back(input);
    }

    // Calcule la taille du cycle et l'indice final dans la liste des états.
    int cycleSize = copyGrid.size() - indexCycle;
    int finalGridIndex = ((PART_2_LIMIT - indexCycle) % cycleSize) + indexCycle;

    // Retourne la charge totale pour l'état final.
    return totalLoad(copyGrid[finalGridIndex]);
}
int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processP1();
    int part2 = processP2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;
    return 0;
}