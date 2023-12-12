/*
  L'entrée représente les statistiques d'un boss d'un RPG

  Dans ce RPG, un marchand propose la vente d'objets avec les caractéristiques suivantes :

  Armes :     Coût Dommages  Armure
  Dagger        8     4       0
  Shortsword   10     5       0
  Warhammer    25     6       0
  Longsword    40     7       0
  Greataxe     74     8       0

  Armures:    Coût Dommages  Armure
  Leather      13     0       1
  Chainmail    31     0       2
  Splintmail   53     0       3
  Bandedmail   75     0       4
  Platemail   102     0       5

  Anneaux:    Coût Dommages  Armure
  Damage +1    25     1       0
  Damage +2    50     2       0
  Damage +3   100     3       0
  Defense +1   20     0       1
  Defense +2   40     0       2
  Defense +3   80     0       3

  Le joueur doit respecter les règles suivantes pour se préparer au combat contre le boss :
  - Il est obligé d'avoir une arme, et seulement une.
  - Il peut choisir au maximum une armure, mais elle n'est pas obligatoire.
  - Il peut choisir entre 0, 1 ou 2 anneaux, cependant, il ne peut pas prendre deux fois le même.

  L'objectif de la partie 1 est de trouver l'équipement le moins cher qui lui permettra de battre le boss. La réponse
  attendue est le coût de l'équipement.

  L'objectif de la partie 2 est de trouver l'équipement le plus cher qui ne permet pas de battre le boss. La réponse
  attendue est le coût de l'équipement.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

class Item
{
  public:
    string name; // Nom de l'objet
    int cost;    // Coût de l'objet
    int damage;  // Dommages infligés par l'objet
    int armor;   // Valeur d'armure fournie par l'objet

    Item(string name, int cost, int damage, int armor)
    {
        this->name = name;
        this->cost = cost;
        this->damage = damage;
        this->armor = armor;
    }
};
class Entity
{
  public:
    int HP;             // Points de vie
    int damage;         // Valeur de Dommages
    int armor;          // Valeur d'armure
    vector<Item> items; // Liste d'objets équipés
    int costItem = 0;   // Coût total des objets équipés

    // Constructeur par défaut
    Entity() = default;

    // Constructeur avec paramètres
    Entity(int HP, int damage, int armor)
    {
        this->HP = HP;
        this->damage = damage;
        this->armor = armor;
    }

    // Méthode pour ajouter un objet à l'inventaire
    void addItem(Item item)
    {
        items.push_back(item);
        damage += item.damage;
        armor += item.armor;
        costItem += item.cost;
    }

    // Opérateur d'affectation
    Entity &operator=(const Entity &other)
    {
        if (this != &other)
        {
            HP = other.HP;
            damage = other.damage;
            armor = other.armor;
            costItem = other.costItem;
            items.clear();
            for (const auto &item : other.items)
            {
                items.push_back(item);
            }
        }
        return *this;
    }

    // Opérateur de comparaison pour la fonction de tri
    bool operator<(const Entity &other) const
    {
        return costItem < other.costItem;
    }
};

// Définition des vecteurs constants d'objets
const vector<Item> WEAPONS = {{"Dagger", 8, 4, 0},
                              {"Shortsword", 10, 5, 0},
                              {"Warhammer", 25, 6, 0},
                              {"Longsword", 40, 7, 0},
                              {"Greataxe", 74, 8, 0}};

const vector<Item> ARMOR = {{"Leather", 13, 0, 1},
                            {"Chainmail", 31, 0, 2},
                            {"Splintmail", 53, 0, 3},
                            {"Bandedmail", 75, 0, 4},
                            {"Platemail", 102, 0, 5}};

const vector<Item> RINGS = {{"Damage +1", 25, 1, 0},  {"Damage +2", 50, 2, 0},  {"Damage +3", 100, 3, 0},
                            {"Defense +1", 20, 0, 1}, {"Defense +2", 40, 0, 2}, {"Defense +3", 80, 0, 3}};

Entity boss;               // Instance de l'entité du boss
vector<Entity> AllPlayers; // Vecteur contenant les différentes configurations d'équipement des joueurs

// Fonction pour choisir les anneaux pour une entité donnée
vector<Entity> chooseRings(Entity player)
{
    vector<Entity> players = {player};

    // Ajout d'un seul anneau
    for (Item ring : RINGS)
    {
        Entity p = player;
        p.addItem(ring);
        players.push_back(p);
    }

    // Ajout de deux anneaux différents
    for (int i = 0; i < RINGS.size(); i++)
    {
        for (int j = i + 1; j < RINGS.size(); j++)
        {
            Entity p = player;
            p.addItem(RINGS[i]);
            p.addItem(RINGS[j]);
            players.push_back(p);
        }
    }

    return players;
}

// Fonction pour choisir l'armure pour une entité donnée
vector<Entity> chooseArmor(Entity player)
{
    vector<Entity> players;

    // Essayer chaque pièce d'armure séparément
    for (Item armor : ARMOR)
    {
        Entity p = player;
        p.addItem(armor);

        // Combiner avec les choix d'anneaux
        vector<Entity> _players = chooseRings(p);
        for (Entity _p : _players)
        {
            players.push_back(_p);
        }
    }

    // Aucune armure choisie
    Entity p = player;
    vector<Entity> _players = chooseRings(p);
    for (Entity _p : _players)
    {
        players.push_back(_p);
    }

    return players;
}

// Fonction pour choisir l'arme
void chooseWaypon()
{
    for (Item weapon : WEAPONS)
    {
        Entity p(100, 0, 0); // Initialiser un joueur avec 100 points de vie et aucun dommage/armure
        p.addItem(weapon);

        // Combiner avec les choix d'armure et d'anneaux
        vector<Entity> _players = chooseArmor(p);
        for (Entity _p : _players)
        {
            AllPlayers.push_back(_p);
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
        vector<string> splitedLine = splitString(line, ": ");

        if (splitedLine[0] == "Hit Points")
        {
            boss.HP = stoi(splitedLine[1]);
        }
        else if (splitedLine[0] == "Damage")
        {
            boss.damage = stoi(splitedLine[1]);
        }
        else if (splitedLine[0] == "Armor")
        {
            boss.armor = stoi(splitedLine[1]);
        }
    }
    file.close();

    chooseWaypon();
    return 0;
}
// Cette fonction simule un combat entre le joueur et le boss, en prenant en compte les points de vie, les dégâts et
// l'armure de chacun.
bool simu(Entity &player)
{
    int bossHP = boss.HP;
    int playerHP = player.HP;

    // Calcul des dégâts infligés par le joueur et le boss en tenant compte de l'armure
    int playerDamage = player.damage - boss.armor;
    int bossDamage = boss.damage - player.armor;

    playerDamage = playerDamage <= 0 ? 1 : playerDamage;
    bossDamage = bossDamage <= 0 ? 1 : bossDamage;

    while (true)
    {
        bossHP -= playerDamage;
        playerHP -= bossDamage;

        // Vérification des points de vie restants
        if (bossHP <= 0)
        {
            return true; // Le joueur a gagné
        }
        else if (playerHP <= 0)
        {
            return false; // Le joueur a perdu
        }
    }
}
// Cette fonction traite la première partie du problème en cherchant la configuration d'équipement la moins chère
// permettant au joueur de battre le boss.
int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Trie les configurations d'équipement par coût croissant
    sort(AllPlayers.begin(), AllPlayers.end());

    // Parcourt les configurations d'équipement pour trouver celle qui permet de battre le boss
    for (Entity p : AllPlayers)
    {
        if (simu(p))
        {
            return p.costItem;
        }
    }

    // Aucune configuration n'a permis de battre le boss
    return -1;
}

// Cette fonction traite la deuxième partie du problème en cherchant la configuration d'équipement la plus chère qui ne
// permet pas au joueur de battre le boss.
int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    // Inverse l'ordre des configurations d'équipement (du plus cher au moins cher)
    reverse(AllPlayers.begin(), AllPlayers.end());

    // Parcourt les configurations d'équipement pour trouver celle qui perd contre le boss
    for (Entity p : AllPlayers)
    {
        if (!simu(p))
        {
            return p.costItem;
        }
    }

    // Toutes les configurations permettent de battre le boss
    return -1;
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