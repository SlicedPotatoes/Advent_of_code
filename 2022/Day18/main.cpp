/*
  L'entrée représente les coordonnées sur une carte en 3D d'un cube de 1x1x1.

  L'ensemble de ces cubes forme une structure fermée qui ressemble à une sphère.

  La première partie consiste à compter le nombre de faces visibles,
  c'est-à-dire que si un cube est isolé, ses 6 faces sont visibles ; si un cube
  a 1 voisins, 5 faces sont visibles, etc.

  La seconde partie consiste à compter le nombre de faces visibles depuis
  l'extérieur.

  J'ai enregistré la structure dans un tableau en 3D et j'ai augmenté chaque coordonnée de 1 pour créer un espace vide
  destiné à accueillir les blocs d'air nécessaires pour la partie 2.

  La première partie est relativement simple, mais la deuxième partie est plus
  complexe à réaliser. La solution que j'ai trouvée consiste à introduire des
  "blocs d'air" dans mon tableau 3D, et je le fais en utilisant une recherche en
  profondeur (DFS). Il me suffit ensuite de parcourir ma liste qui contient
  toutes les coordonnées des blocs initiaux et de compter le nombre de voisins
  qui sont des "blocs d'air" pour obtenir le nombre de faces visibles depuis
  l'extérieur.
*/

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Structure permettant de stocker un point sur un plan 3D.
struct Point
{
    int x; // Coordonnée en X.
    int y; // Coordonnée en Y.
    int z; // Coordonnée en Z.
};

const char EMPTY_SLOT_MAP = '.';  // Caractère représentant un emplacement vide sur la carte.
const char FILLED_SLOT_MAP = '#'; // Caractère représentant un emplacement occupé sur la carte.
const char AIR_BLOCK = '@';       // Caractère représentant un bloc d'air sur la carte.

// Vecteur contenant les déplacements dans les six directions possibles en 3D.
const vector<Point> dir = {
    {-1, 0, 0}, // Déplacement vers la gauche.
    {1, 0, 0},  // Déplacement vers la droite.
    {0, -1, 0}, // Déplacement vers le haut.
    {0, 1, 0},  // Déplacement vers le bas.
    {0, 0, 1},  // Déplacement vers l'avant.
    {0, 0, -1}  // Déplacement vers l'arrière.
};

// Fonction qui split un string en vector<string> en utilisant un délimiteur.
vector<string> splitString(const string &input, const string &delimiter)
{
    vector<string> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        if (end != start)
        {
            tokens.push_back(input.substr(start, end - start));
        }
        start = end + delimiter.length();
    }

    if (start < input.length())
    {
        tokens.push_back(input.substr(start));
    }

    return tokens;
}

char ***array3D;     // Stocke la carte 3D.
vector<Point> input; // Stocke les coordonnées d'entrée du puzzle.
int maxCoord = 0;    // Stocke la valeur de la coordonnée la plus grande.

// Vérifie si les coordonnées (x, y, z) se trouvent à l'intérieur des limites du tableau 3D.
bool inArrayLimit(int x, int y, int z)
{
    return (x >= 0 && x < maxCoord && y >= 0 && y < maxCoord && z >= 0 && z < maxCoord);
}

// Renvoie un vecteur de points voisins valides pour le point donné 'p' en utilisant les déplacements définis dans le
// vecteur 'dir'.
vector<Point> getNeighbor(Point p)
{
    vector<Point> v;
    for (Point d : dir)
    {
        int x = p.x + d.x;
        int y = p.y + d.y;
        int z = p.z + d.z;

        if (inArrayLimit(x, y, z))
        {
            v.push_back({x, y, z});
        }
    }
    return v;
}

// Parcours en profondeur (DFS) pour remplir les "blocs d'air" depuis le point 'p' dans le tableau 3D.
void dfs(Point p)
{
    vector<Point> neighbor = getNeighbor(p);
    for (Point n : neighbor)
    {
        if (array3D[n.x][n.y][n.z] == EMPTY_SLOT_MAP)
        {
            array3D[n.x][n.y][n.z] = AIR_BLOCK;
            dfs(n);
        }
    }
}

// Initialise le tableau 3D avec des valeurs par défaut et les valeurs d'entrée,
// puis effectue un parcours en profondeur (DFS) pour remplir les "blocs d'air".
void initArray3D()
{
    auto start = chrono::high_resolution_clock::now();

    // Alloue de la mémoire pour le tableau 3D
    array3D = new char **[maxCoord];
    for (int i = 0; i < maxCoord; i++)
    {
        array3D[i] = new char *[maxCoord];
        for (int j = 0; j < maxCoord; j++)
        {
            array3D[i][j] = new char[maxCoord];
            for (int k = 0; k < maxCoord; k++)
            {
                array3D[i][j][k] = EMPTY_SLOT_MAP; // Initialise avec des emplacements vides par défaut
            }
        }
    }

    // Remplit le tableau 3D avec les valeurs d'entrée
    for (Point pos : input)
    {
        array3D[pos.x][pos.y][pos.z] = FILLED_SLOT_MAP;
    }

    // Effectue un parcours en profondeur (DFS) pour remplir les "blocs d'air"
    dfs({0, 0, 0});
    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Temps d'exécution InitArray3D : " << time.count() << " μs\n";
}
// Calcule et affiche le résultat de la partie 1 : le nombre de faces visibles.
void processP1()
{
    auto start = chrono::high_resolution_clock::now();
    int result = 0;
    for (Point p : input)
    {
        vector<Point> neighbor = getNeighbor(p);
        for (Point n : neighbor)
        {
            if (array3D[n.x][n.y][n.z] != FILLED_SLOT_MAP)
            {
                result++;
            }
        }
    }
    cout << "Part1: " << result << "\n";
    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Temps d'exécution : " << time.count() << " μs\n";
}

// Calcule et affiche le résultat de la partie 2 : le nombre de faces visibles depuis l'extérieur.
void processP2()
{
    auto start = chrono::high_resolution_clock::now();
    int result = 0;
    for (Point p : input)
    {
        vector<Point> neighbor = getNeighbor(p);
        for (Point n : neighbor)
        {
            if (array3D[n.x][n.y][n.z] == AIR_BLOCK)
            {
                result++;
            }
        }
    }
    cout << "Part2: " << result << "\n";
    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Temps d'exécution : " << time.count() << " μs\n";
}

int main()
{
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
        vector<string> splitedString = splitString(line, ",");
        Point p = {0, 0, 0};
        for (int i = 0; i < 3; i++)
        {
            int value = stoi(splitedString[i]) + 1;
            if (i == 0)
            {
                p.x = value;
            }
            else if (i == 1)
            {
                p.y = value;
            }
            else
            {
                p.z = value;
            }
            if (value > maxCoord)
            {
                maxCoord = value;
            }
        }
        input.push_back(p);
    }
    file.close();
    maxCoord += 2;

    initArray3D();
    processP1();
    processP2();

    return 0;
}