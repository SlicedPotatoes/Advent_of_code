/*
  L'entrée représente une liste de grilles.

  Objectif de la Partie 1: Trouver un axe de symétrie dans la grille (vertical ou horizontal).
  Il faut retourner la somme de l'indice de chaque axe.
  Si l'axe est sur une colonne, son indice est ajouté directement à la somme. Si sur une ligne, on multiplie l'indice
  par 100.

  Objectif de la Partie 2: Trouver un élément à modifier dans la grille pour obtenir un axe différent de celui
  trouvé dans la Partie 1.
  Il est important de noter que l'axe d'origine peut être encore présent après une modification.
  Dans ce cas, si un deuxième axe existe, seul le deuxième axe doit être pris en compte.
*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

struct Input
{
    vector<string> data;
    vector<string> rotatedData;
    int lastLineValue = -1;
    int lastColValue = -1;
};

vector<Input> inputs;

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
    vector<string> arr; // Vecteur temporaire pour stocker les lignes de chaque grille
    while (getline(file, line))
    {
        if (line == "")
        {
            // Ajout des données de la grille actuelle au vecteur d'entrées
            inputs.push_back({arr});
            arr.clear();
            continue;
        }
        arr.push_back(line);
    }
    // Ajout des données de la dernière grille au vecteur d'entrées
    inputs.push_back({arr});
    file.close();

    // Rotation des données pour obtenir la version transposée de chaque grille
    for (Input &i : inputs)
    {
        vector<string> data(i.data[0].size(), "");
        for (int indexString = 0; indexString < i.data.size(); indexString++)
        {
            for (int indexChar = 0; indexChar < i.data[0].size(); indexChar++)
            {
                data[indexChar].push_back(i.data[indexString][indexChar]);
            }
        }
        i.rotatedData = data;
    }

    return 0;
}

// Fonction pour vérifier si une chaîne est un palindrome
bool checkPalindrome(string s, int pivot, int size)
{
    for (int i = 1; i <= size; i++)
    {
        // Comparaison des caractères symétriques par rapport au pivot
        if (s[pivot - i] != s[pivot + i - 1])
        {
            return false; // La chaîne n'est pas un palindrome
        }
    }
    return true; // La chaîne est un palindrome
}

// Fonction pour trouver les indices des pivots symétriques dans les grilles données
vector<int> findPalindrome(vector<string> data)
{
    vector<int> pivots; // Vecteur pour stocker les indices des pivots symétriques

    // Parcours des indices de pivot possibles
    for (int pivot = 1; pivot < data[0].size(); pivot++)
    {
        int maxSize = min(pivot, int(data[0].size() - pivot)); // Taille maximale possible pour la vérification
        bool valid = true;                                     // Variable pour vérifier la validité du pivot actuel

        // Vérification de symétrie pour chaque chaîne de la grille
        for (string &s : data)
        {
            if (!checkPalindrome(s, pivot, maxSize))
            {
                valid = false; // La symétrie n'est pas maintenue pour une chaîne, on arrête la vérification
                break;
            }
        }

        // Si le pivot est valide pour toutes les chaînes, on l'ajoute au vecteur des pivots
        if (valid)
        {
            pivots.push_back(pivot);
        }
    }
    return pivots; // Retourne le vecteur des pivots symétriques
}

// Fonction pour traiter la Partie 1 du problème
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int sum = 0; // Variable pour stocker la somme des indices d'axe de symétrie

    // Parcours des éléments d'entrée
    for (Input &element : inputs)
    {
        vector<int> col = findPalindrome(element.data); // Recherche d'axes de symétrie sur les colonnes

        // Si aucun axe de symétrie sur les colonnes n'est trouvé
        if (col.size() == 0)
        {
            vector<int> line = findPalindrome(element.rotatedData); // Recherche d'axes de symétrie sur les lignes
            element.lastLineValue = line[0];                        // Stockage de la valeur de l'indice de ligne
            sum += line[0] * 100;                                   // Ajout à la somme en multipliant par 100
            continue;
        }
        element.lastColValue = col[0]; // Stockage de la valeur de l'indice de colonne
        sum += col[0];                 // Ajout à la somme
    }

    return sum; // Retourne la somme des indices d'axe de symétrie
}

// Fonction pour traiter la Partie 2 du problème
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME
    int sum = 0; // Variable pour stocker la somme des indices d'axe de symétrie après modification

    // Parcours des éléments d'entrée
    for (Input &element : inputs)
    {
        bool end = false; // Variable pour signaler la fin de la recherche d'axes symétriques après modification

        // Parcours des coordonnées de la grille
        for (int y = 0; y < element.data.size(); y++)
        {
            if (end)
            {
                break; // Si la recherche est terminée, sortir de la boucle externe
            }
            for (int x = 0; x < element.data[0].size(); x++)
            {
                char defaultChar = '.'; // Caractère par défaut pour la restauration après modification

                // Modification de la grille en inversant le caractère à la position courante
                if (element.data[y][x] == '.')
                {
                    element.data[y][x] = '#';
                    element.rotatedData[x][y] = '#';
                }
                else
                {
                    element.data[y][x] = '.';
                    element.rotatedData[x][y] = '.';
                    defaultChar = '#';
                }

                // Recherche d'axes de symétrie après modification
                vector<int> col = findPalindrome(element.data);
                vector<int> line = findPalindrome(element.rotatedData);

                // Vérification des nouveaux axes de symétrie par rapport aux anciens
                for (int c : col)
                {
                    if (c != element.lastColValue)
                    {
                        sum += c;   // Ajout à la somme
                        end = true; // Fin de la recherche après modification
                    }
                }
                for (int l : line)
                {
                    if (l != element.lastLineValue)
                    {
                        sum += l * 100; // Ajout à la somme en multipliant par 100
                        end = true;     // Fin de la recherche après modification
                    }
                }

                // Restauration de la grille à son état initial
                element.data[y][x] = defaultChar;
                element.rotatedData[x][y] = defaultChar;

                if (end)
                {
                    break; // Si la recherche est terminée, sortir de la boucle interne
                }
            }
        }
    }

    return sum; // Retourne la somme des indices d'axe de symétrie après modification
}
int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    int part1 = processPart1();
    int part2 = processPart2();

    cout << "\nPart1: " << part1;
    cout << "\nPart2: " << part2;

    return 0;
}