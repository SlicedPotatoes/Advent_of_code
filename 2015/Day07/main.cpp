/*
  L'entrée du programme consiste en un ensemble d'instructions décrivant la connexion d'un système de portes logiques.
  Chaque fil dans ce système est identifié de manière unique et peut transporter un signal de 16 bits

  Chaque ligne d'instruction spécifie comment connecter les composants entre eux.
  Par exemple, l'instruction "x AND y -> z" indique de connecter les fils x et y à une porte logique AND, et de
  connecter la sortie au fil z.

  Dans la première partie, l'objectif est de déterminer la valeur du fil "a".

  Pour la seconde partie, le processus implique la réinitialisation de toutes les valeurs, l'initialisation du fil "b"
  avec la valeur précédemment trouvée pour "a", et enfin, la recherche de la nouvelle valeur du fil "a".
*/

#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Structure représentant les fils connectés à la porte logique "opr"
struct Parent
{
  public:
    string p1;  // Premier fil connecté
    string p2;  // Deuxième fil connecté
    string opr; // Opération logique réalisée par la porte
};
// Structure représentant une porte logique avec ses valeurs d'entrée et de sortie
struct Node
{
  public:
    Parent input;           // Entrées de la porte logique avec l'opération
    string name;            // Nom du fil de sortie de la porte logique
    uint16_t value;         // Valeur de sortie de la porte logique (16 bits non signés)
    bool completed = false; // Indicateur de complétion, true si la valeur a été calculée
    Node() = default;
};

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

// Table de hachage représentant un arbre de noeuds logiques
unordered_map<string, Node> tree;

bool isNumeric(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

/**
 * Obtient les informations sur les parents d'un noeud logique à partir de la chaîne 's'.
 *
 * @param s Chaîne de caractères représentant les parents potentiels d'un noeud logique.
 * @return Une structure Parent contenant les informations sur les parents.
 */
Parent getParent(string &s)
{
    // si 's' est une valeur numérique, retourne 's' comme parent1 avec l'opération "number"
    if (isNumeric(s))
    {
        return {s, "", "number"};
    }
    vector<string> sSplited = splitString(s, " ");
    // Si 's' est une référence unique, retourne 's' comme parent1 avec l'opération "ref"
    if (sSplited.size() == 1)
    {
        return {s, "", "ref"};
    }
    // Si 's' commence par "NOT", retourne la seconde valeur du vecteur sSplited avec pour opération "NOT"
    if (sSplited[0] == "NOT")
    {
        return {sSplited[1], "", "NOT"};
    }
    // Par défaut, renvoie les deux premières parties comme parents et la troisième comme opération
    return {sSplited[0], sSplited[2], sSplited[1]};
}

/**
 * Calcule la valeur de sortie d'un noeud logique en fonction de ses parents et de l'opération spécifiée.
 *
 * @param p Une structure Parent représentant les parents du noeud logique et l'opération à effectuer.
 * @param output Le nom du fil de sortie où stocker le résultat du calcul.
 */
void computeValue(Parent &p, string &output)
{
    // Obtient les valeurs des parents en fonction de leur nature (numérique ou provenant d'un autre noeud logique)
    uint16_t value1 = isNumeric(p.p1) ? stoi(p.p1) : tree[p.p1].value;
    uint16_t value2 = p.p2 == "" ? 0 : (isNumeric(p.p2) ? stoi(p.p2) : tree[p.p2].value);

    // Vérifie si les parents ont été complétés avant de procéder au calcul
    if (!isNumeric(p.p1) && !tree[p.p1].completed)
    {
        return;
    }
    if (p.p2 != "" && !isNumeric(p.p2) && !tree[p.p2].completed)
    {
        return;
    }

    // Effectue le calcul en fonction de l'opération spécifiée et stocke le résultat dans le fil de sortie
    if (p.opr == "number" || p.opr == "ref")
    {
        tree[output].value = value1;
    }
    else if (p.opr == "NOT")
    {
        tree[output].value = ~value1;
    }
    else if (p.opr == "AND")
    {
        tree[output].value = value1 & value2;
    }
    else if (p.opr == "OR")
    {
        tree[output].value = value1 | value2;
    }
    else if (p.opr == "LSHIFT")
    {
        tree[output].value = value1 << value2;
    }
    else if (p.opr == "RSHIFT")
    {
        tree[output].value = value1 >> value2;
    }

    // Marque le fil de sortie comme complété
    tree[output].completed = true;
}

/**
 * Parcours en profondeur (Depth-First Search) pour calculer les valeurs des noeuds logiques.
 * Si le noeud en question est déjà complété, la fonction retourne immédiatement.
 * Sinon, elle effectue un DFS sur les parents non complétés du noeud, puis calcule la valeur du noeud.
 *
 * @param n Le noeud logique pour lequel calculer la valeur.
 */
void dfs(Node &n)
{
    // Si le noeud est déjà complété, retourne immédiatement
    if (n.completed)
    {
        return;
    }
    // Si le premier n'est pas une valeur numérique, et n'est pas complété, effectue un DFS récursif sur ce parent
    if (!isNumeric(n.input.p1) && !tree[n.input.p1].completed)
    {
        dfs(tree[n.input.p1]);
    }
    // Si le deuxième parent existe, n'est pas une valeur numérique, et n'est pas complété, effectue un DFS récursif sur
    // ce parent
    if (n.input.p2 != "" && !isNumeric(n.input.p2) && !tree[n.input.p2].completed)
    {
        dfs(tree[n.input.p2]);
    }
    // Calcule la valeur du noeud en utilisant la fonction computeValue
    computeValue(n.input, n.name);
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

    auto startTime = chrono::steady_clock::now();

    while (getline(file, line))
    {
        vector<string> splitedLine = splitString(line, " -> ");
        Parent p = getParent(splitedLine[0]);
        Node n = {};
        n.input = p;
        n.name = splitedLine[1];
        tree[splitedLine[1]] = n;
    }

    auto endTime = chrono::steady_clock::now();
    auto parsingTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    file.close();

    startTime = chrono::steady_clock::now();

    dfs(tree["a"]);

    uint16_t part1 = tree["a"].value;

    endTime = chrono::steady_clock::now();
    auto part1Time = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    startTime = chrono::steady_clock::now();

    for (auto &it : tree)
    {
        it.second.completed = false;
    }
    tree["b"].value = part1;
    tree["b"].completed = true;
    dfs(tree["a"]);

    endTime = chrono::steady_clock::now();
    auto part2Time = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    cout << "Part1: " << part1 << '\n';
    cout << "Part2: " << tree["a"].value << "\n\n";

    cout << "Parsing: " << parsingTime.count() << "μs\n";
    cout << "Part1: " << part1Time.count() << "μs\n";
    cout << "Part2: " << part2Time.count() << "μs\n";

    return 0;
}