/*

  Notre tante Sue nous a offert un cadeau, et nous aimerions lui envoyer une carte de remerciement.
  Cependant, il y a un petit problème : elle a signé le cadeau "De tante Sue". Le hic, c'est que nous avons 500 tantes
  "Sue".

  Pour envoyer la carte à la bonne tante, nous devons déterminer laquelle nous a envoyé le cadeau.
  Par chance, nous avons un appareil révolutionnaire qui permet de détecter des composés spécifiques dans un
  échantillon.

  Cet appareil permet de détecter:
  - Le nombre d'enfants
  - Le nombre de chats
  - Le nombre de chiens avec leur race (samoyeds, pomeranians, akitas, vizslas)
  - Le nombre de poissons rouges
  - Le nombre d'arbres
  - Le type de voiture
  - Les parfums

  En analysant le paquet cadeau, nous obtenons ce compte rendu:

  children: 3
  cats: 7
  samoyeds: 2
  pomeranians: 3
  akitas: 0
  vizslas: 0
  goldfish: 5
  trees: 3
  cars: 2
  perfumes: 1

  Nous avons donc établi une liste de nos 500 tantes Sue avec les éléments dont nous nous souvenons à leur sujet.
  Les éléments manquants dans notre liste ne sont pas nuls, nous ne nous en souvenons simplement plus.

  Dans la partie 1, le but est de retrouver l'identifiant de la tante Sue qui nous a envoyé le cadeau.

  Dans la partie 2, nous remarquons une petite note écrite sur la notice de la machine:
  Le résultat est imprécis pour plusieurs éléments qui peuvent être détectés:
  - Les chats et les arbres, il faut partir du principe que leur nombre sera supérieur à la valeur de détection.
  - Les Poméraniens et les poissons rouges, il faut partir du principe que leur nombre sera inférieur à la valeur de
  détection.
  L'objectif est de trouver quelle tante Sue respecte ces conditions.

*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

const int EMPTY_DATA = -1; // Valeur par défaut pour les données manquantes

// Liste des clés pour les éléments détectés par l'appareil
const vector<string> SUE_KEY = {"children", "cats",     "samoyeds", "pomeranians", "akitas",
                                "vizslas",  "goldfish", "trees",    "cars",        "perfumes"};

// Les valeurs sorties par l'appareil
unordered_map<string, int> TARGET_SUE = {{"children", 3}, {"cats", 7},    {"samoyeds", 2}, {"pomeranians", 3},
                                         {"akitas", 0},   {"vizslas", 0}, {"goldfish", 5}, {"trees", 3},
                                         {"cars", 2},     {"perfumes", 1}};

vector<unordered_map<string, int>> sues; // Liste des profils des tantes Sue

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
        line = line.substr(line.find(':') + 2);
        vector<string> splitedInput = splitString(line, ", ");
        unordered_map<string, int> oneSue;

        // Initialisation des données pour une tante Sue
        for (string s : SUE_KEY)
        {
            oneSue[s] = EMPTY_DATA;
        }

        // Analyse et stockage des données spécifiques de la tante Sue
        for (string s : splitedInput)
        {
            vector<string> _s = splitString(s, ": ");
            oneSue[_s[0]] = stoi(_s[1]);
        }

        sues.push_back(oneSue); // Ajout des données de la tante Sue à la liste
    }
    file.close();

    return 0;
}

int processPart1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int index = 1;

    // Parcours de la liste des tantes Sue
    for (unordered_map<string, int> oneSue : sues)
    {
        bool valid = true;

        // Vérification des caractéristiques de la tante Sue
        for (string key : SUE_KEY)
        {
            // Si la caractéristique est spécifiée et ne correspond pas à celle de la tante cible, la tante actuelle
            // n'est pas valide
            if (oneSue[key] != -1 && oneSue[key] != TARGET_SUE[key])
            {
                valid = false;
                break;
            }
        }

        // Si la tante actuelle est valide, retour de son index
        if (valid)
        {
            return index;
        }
        index++;
    }
    return 0;
}

int processPart2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    int index = 1;

    // Parcours de la liste des tantes Sue
    for (unordered_map<string, int> oneSue : sues)
    {
        bool valid = true;

        // Vérification des caractéristiques de la tante Sue avec des ajustements pour les éléments spécifiques
        for (string key : SUE_KEY)
        {
            if (key == "cats" || key == "trees")
            {
                // Si le nombre de chats ou d'arbres est spécifié et inférieur ou égal à celui de la tante cible, la
                // tante actuelle n'est pas valide
                if (oneSue[key] != -1 && oneSue[key] <= TARGET_SUE[key])
                {
                    valid = false;
                    break;
                }
            }
            else if (key == "pomeranians" || key == "goldfish")
            {
                // Si le nombre de Poméraniens ou de poissons rouges est spécifié et supérieur ou égal à celui de la
                // tante cible, la tante actuelle n'est pas valide
                if (oneSue[key] != -1 && oneSue[key] >= TARGET_SUE[key])
                {
                    valid = false;
                    break;
                }
            }
            // Pour les autres caractéristiques, vérifier si elles correspondent à celles de la tante cible
            else if (oneSue[key] != -1 && oneSue[key] != TARGET_SUE[key])
            {
                valid = false;
                break;
            }
        }

        // Si la tante actuelle est valide, retour de son index
        if (valid)
        {
            return index;
        }
        index++;
    }
    return 0;
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