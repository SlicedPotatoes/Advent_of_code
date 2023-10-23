/*
  L'input représente un réseau de valve, où chaque valve possède un débit (flow rate) et des voisins.
  Selon l'énoncé, l'ouverture d'une valve prend une minute, et se déplacer d'une valve à une autre prend également une minute.

  Dans la partie 1, l'objectif est de déterminer la pression maximale que l'on peut relâcher en partant de la valve AA en 30 minutes.

  Dans la partie 2, nous avons un éléphant qui nous assiste, mais il faut lui consacrer 4 minutes pour lui apprendre comment se déplacer dans ce réseau de valves et les ouvrir.
  L'objectif est toujours le même: trouver la pression maximale que l'on peut relâcher en partant de la valve AA en 30mn (- 4mn pour l'apprentissage) avec l'aide de l'éléphant.

  Mon programme utilise un algorithme glouton (ou greedy): Beam Search.
  L'idée que j'ai eue est de créer un arbre en commençant de AA à la minute 0.
  À chaque instant t, je tiens compte de toutes les actions possibles que je stocke dans des nœuds (Nodes).
  Sans l'implémentation de la méthode "beam search", cette approche prendrait des jours pour s'exécuter.
  Cependant, grâce à une fonction heuristique, cela permet de supprimer les cas dits "irrattrapable".
*/
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

#include <chrono>

using namespace std;

const int MAX_DEPTH_P1 = 30; // Profondeur maximale d'exploration de l'arbre de solution pour P1.
const int MAX_DEPTH_P2 = 26; // Profondeur maximale d'exploration de l'arbre de solution pour P2.
const int BEAM_WIDTH = 1500; // Taille du BeamWidth.
const double POID_HEURISTIQUE = 0.8;

// Classe permettant de stocker les informations d'une valve.
class Valve
{
public:
  string name;
  int flowRate;
  vector<string> neighboor;
  Valve() {}
  Valve(string name, int flowRate, vector<string> neighboor)
  {
    this->name = name;
    this->flowRate = flowRate;
    this->neighboor = neighboor;
  }
  bool operator<(const Valve &a) const
  {
    return flowRate > a.flowRate;
  }
};

vector<Valve> graphValves;                 // Vecteur permettant de stocker le graphe des valves.
unordered_map<string, int> nameIndexValve; // Map qui associe le nom d'une Valve à son indice.

unordered_map<string, int> cacheDistance; // Cache de la fonction "distance" pour éviter de recalculer une valeur déjà obtenue précédemment.

// Classe Node permettant de stocker un noeud de l'arbre de résolution.
class Node
{
public:
  int score;               // Score actuel de la node.
  int heuristique;         // Score potentiel maximal.
  vector<string> opened;   // Liste des valves ouvertes.
  Valve position;          // Position actuelle dans le graphe des valves.
  Valve position2;         // Position actuelle de l'éléphant.
  int depth;               // Profondeur.
  vector<string> registre; // Registre des actions menant à ce point (Utilisé pendant le débogage).
  bool isP1;               // Boolean indiquant si le noeud est utilisé pour la partie 1 ou la partie 2.
  // Constructeur utilisé pour la partie1.
  Node(int score, Valve position)
  {
    this->depth = 0;
    this->score = score;
    this->position = position;
    this->isP1 = true;
    registre.push_back("Depth: 0, Initialisé");
  }
  // Constructeur utilisé pour la partie2.
  Node(int score, Valve position1, Valve position2)
  {
    this->depth = 0;
    this->score = score;
    this->position = position1;
    this->position2 = position2;
    this->isP1 = false;
    registre.push_back("Depth: 0, Initialisé");
  }
  // Méthode qui renvoie true si la position actuelle est une valve fermée.
  bool myPosCanBeOpen(bool isElephant)
  {
    return find(opened.begin(), opened.end(), (isElephant ? position2.name : position.name)) == opened.end();
  }
  // Méthode qui ajoute la valve de la position actuelle à la liste "opened" et incrémente le score en fonction de la profondeur actuelle de la node.
  void openValve(bool isElephant)
  {
    registre.push_back("Depth: " + to_string(depth) + " Open" + (isElephant ? " Elephant" : "") + ": " + (isElephant ? position2.name : position.name) + " score: " + to_string(score));
    score += (isElephant ? position2.flowRate : position.flowRate) * ((isP1 ? MAX_DEPTH_P1 : MAX_DEPTH_P2) - depth);
    opened.push_back((isElephant ? position2.name : position.name));
  }
  /*
    Méthode qui calcule l'heuristique.
    Le principe est simple: nous simulons le score maximum que nous pourrions obtenir si nous alignons toutes les valves fermées (dans l'ordre décroissant de leur débit)
    et que nous les ouvrons une par une.
    On incrémente l'heuristique en multipliant par POID_HEURISTIQUE l'ajout de score que l'on obtient en ouvrant la valve pour que celui-ci est moins d'importance que le score actuel.
  */
  void calculHeuristique()
  {
    heuristique = score;    // Initialisez l'heuristique avec le score actuel.
    vector<string> _opened; // Liste temporaire des valves ouvertes pendant le calcul de l'heuristique.
    // Simule depuis la profondeur actuelle jusqu'à la profondeur maximale.
    for (int _depth = depth; _depth <= (isP1 ? MAX_DEPTH_P1 : MAX_DEPTH_P2); _depth += 2)
    {
      bool end = true;
      // On parcourt la liste des valves qui sont triées dans l'ordre décroissant par rapport à leur débit.
      for (int i = 0; i < graphValves.size(); i++)
      {
        bool notInOpened = find(opened.begin(), opened.end(), graphValves[i].name) == opened.end();
        bool notInTempOpened = find(_opened.begin(), _opened.end(), graphValves[i].name) == _opened.end();
        // Si la valve n'est pas ouverte dans la liste temporaire ni dans la liste "opened", on incrémente le score
        if (notInOpened && notInTempOpened)
        {
          end = false;
          heuristique += ((isP1 ? MAX_DEPTH_P1 : MAX_DEPTH_P2) - _depth) * graphValves[i].flowRate * POID_HEURISTIQUE;
          _opened.push_back(graphValves[i].name);
        }
      }
      // Toutes les valves sont ouvertes, on quitte la boucle.
      if (end)
      {
        break;
      }
    }
  }
  bool operator<(const Node &a) const
  {
    return heuristique < a.heuristique;
  }
};

// Fonction qui fais une deepCopy d'une Node.
Node deepCopy(const Node &n)
{
  Node _n = {n.score, n.position};
  if (!n.isP1)
  {
    _n = {n.score, n.position, n.position2};
  }

  _n.depth = n.depth + 1;
  for (int i = 0; i < n.opened.size(); i++)
  {
    _n.opened.push_back(n.opened[i]);
  }
  for (int i = 1; i < n.registre.size(); i++)
  {
    _n.registre.push_back(n.registre[i]);
  }
  return _n;
}

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

// Simulation des déplacements de l'éléphant
vector<Node> elephantMove(Node n)
{
  vector<Node> list;
  if (n.myPosCanBeOpen(true))
  {
    Node _n = deepCopy(n);
    _n.depth = n.depth;
    _n.openValve(true);
    _n.calculHeuristique();
    list.push_back(_n);
  }
  for (int i = 0; i < n.position2.neighboor.size(); i++)
  {
    int valveIndex = nameIndexValve[n.position2.neighboor[i]];
    Valve v = graphValves[valveIndex];
    if (v.name != n.position.name)
    {
      Node _n = deepCopy(n);
      _n.depth = n.depth;
      _n.position2 = v;
      _n.calculHeuristique();
      _n.registre.push_back("Depth: " + to_string(_n.depth) + " Move Elephant: " + v.name + " score: " + to_string(_n.score));
      list.push_back(_n);
    }
  }
  return list;
}
/*
  Fonction permettant d'obtenir une solution "Optimale Locale".
  Cependant, en utilisant la bonne heuristique et une largeur de faisceau (beam width) appropriée,
  il est possible d'obtenir une solution "Optimale Globale" à notre problème.
*/
int simulate(bool isP1)
{
  priority_queue<Node> list; // PriorityQueue des nodes a explorer.

  Node startNode = {0, graphValves[nameIndexValve["AA"]]}; // Création du noeud de départ.
  if (!isP1)
  {
    startNode = {0, graphValves[nameIndexValve["AA"]], graphValves[nameIndexValve["AA"]]}; // Si on est en P2.
  }

  startNode.calculHeuristique();
  list.push(startNode); // Ajout de la node de départ à la file de priorité.

  int maxScore = 0;         // Enregistre le score maximum.
  Node maxNode = startNode; // Stocke la node ayant le score maximal.

  while (!list.empty())
  {
    cout << list.top().depth << "|";
    priority_queue<Node> _list;                                              // Création d'une priority queue temporaire.
    int nbIteration = (list.size() < BEAM_WIDTH ? list.size() : BEAM_WIDTH); // Déterminer le nombre d'itérations.
    // On parcourt la priority queue
    for (int indexQueue = 0; indexQueue < nbIteration; indexQueue++)
    {
      // On extrait le noeud ayant la priorité la plus élevée de la file d'attente et le retiron de celle-ci.
      Node currentNode = list.top();
      list.pop();

      // Si le score de la Node est plus élevé que maxScore, mettre ce score dans maxScore et la node dans maxNode.
      if (currentNode.score > maxScore)
      {
        maxScore = currentNode.score;
        maxNode = currentNode;
      }
      /*
        On arrête de parcourir cette branche si l'une des conditions suivantes est remplie:
        - Le score est égal à l'heuristique.
        - La profondeur est égale à MAX_DEPTH.
        - Le noeud actuel est "irrattrapable".
      */
      if (currentNode.score == currentNode.heuristique || currentNode.depth == (isP1 ? MAX_DEPTH_P1 : MAX_DEPTH_P2) || currentNode.heuristique < maxScore)
      {
        continue;
      }
      // Si la Valve de notre Node peut être ouverte, on crée une nouvelle Node avec cette Valve ouverte qu'on ajoute à la priority queue temporaire.
      if (currentNode.myPosCanBeOpen(false))
      {
        Node n = deepCopy(currentNode);
        n.openValve(false);
        // Si on est dans la partie 1, on ajoute notre node a la priority queue temporaire.
        if (isP1)
        {
          n.calculHeuristique();
          _list.push(n);
        }
        // Sinon on calcul les déplacements de l'elephant et on ajoute toutes les nodes a la priority queue temporaire.
        else
        {
          vector<Node> moves = elephantMove(n);
          for (int i = 0; i < moves.size(); i++)
          {
            _list.push(moves[i]);
          }
        }
      }
      // On parcourt les voisins de la Valve à notre position, on crée une nouvelle Node avec le déplacement vers le voisin. Ensuite, on l'ajoute à la priority queue temporaire.
      for (int i = 0; i < currentNode.position.neighboor.size(); i++)
      {
        Node n = deepCopy(currentNode);
        int valveIndex = nameIndexValve[currentNode.position.neighboor[i]];
        Valve v = graphValves[valveIndex];
        n.position = v;
        n.registre.push_back("Depth: " + to_string(n.depth) + " Move: " + n.position.name + " score: " + to_string(n.score));
        // Si on est dans la partie 1, on ajoute notre node a la priority queue temporaire.
        if (isP1)
        {
          n.calculHeuristique();
          _list.push(n);
        }
        // Sinon on calcul les déplacements de l'elephant et on ajoute toutes les nodes a la priority queue temporaire.
        else
        {
          vector<Node> moves = elephantMove(n);
          for (int i = 0; i < moves.size(); i++)
          {
            _list.push(moves[i]);
          }
        }
      }
    }
    list = _list; // On remplace notre list par la liste temporaire.
  }
  cout << "\n";
  for (int i = 0; i < maxNode.registre.size(); i++)
  {
    cout << maxNode.registre[i] << "\n";
  }
  cout << "Depth: " << maxNode.depth << " End score: " << maxNode.score << " Heuristique: " << maxNode.heuristique << "\n";
  return maxScore;
}

int main()
{
  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day16\\input.txt";
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
    return 1;
  }
  string line;
  while (getline(file, line))
  {
    regex pattern("Valve (\\w+) has flow rate=(\\d+);\\s*(?:tunnels?|tunnel) (?:leads?|lead) to valves? (.+)");
    smatch matches;
    if (regex_search(line, matches, pattern))
    {
      string name = matches[1];
      int flowRate = stoi(matches[2]);
      vector<string> outputs = splitString(matches[3], ", ");
      graphValves.push_back({name, flowRate, outputs});
    }
    else
    {
      cout << line << " No Match\n";
      return -1;
    }
  }
  file.close();
  sort(graphValves.begin(), graphValves.end());
  for (int i = 0; i < graphValves.size(); i++)
  {
    Valve v = graphValves[i];
    nameIndexValve[v.name] = i;
  }

  cout << "Start P1, DEPTH: ";
  auto start = chrono::high_resolution_clock::now();
  int part1 = simulate(true);
  auto stop = chrono::high_resolution_clock::now();
  auto timeP1 = chrono::duration_cast<chrono::microseconds>(stop - start);

  cout << "Start P2, DEPTH: ";
  start = chrono::high_resolution_clock::now();
  int part2 = simulate(false);
  stop = chrono::high_resolution_clock::now();
  auto timeP2 = chrono::duration_cast<chrono::microseconds>(stop - start);

  cout << "Part1: " << part1 << "\n";
  cout << "Temps d'exécution : " << timeP1.count() << " μs\n";

  cout << "Part2: " << part2 << "\n";
  cout << "Temps d'exécution : " << timeP2.count() << " μs\n";
  return 0;
}