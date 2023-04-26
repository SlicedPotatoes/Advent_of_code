/*
L'input représente une liste de singes qui ont plusieurs attributs:
- Une liste d'objets où le nombre représente mon "niveau d'inquiétude"
- Une opération qui sera appliquée sur l'objet
- Un test qui déterminera à quel singe le singe actuel va donner l'objet
Nous devons ensuite simuler des "rounds", où chaque singe s'amusera avec tous les objets un par un.
Cela aura pour effet d'augmenter mon "niveau d'inquiétude" (en appliquant l'opération propre à ce singe),
puis le singe passera l'objet à un autre singe en fonction du test.
Pour chaque singe, nous comptons le nombre de fois où il s'est amusé avec un objet, que nous appellerons le "Monkey Business".
Pour chaque partie, il faut retourner le produit du "Monkey Business" des 2 singes ayant le "Monkey Business" le plus élevé.
Pour la partie 1, il faut simuler 20 rounds pour chaque objet, avant d'effectuer le test quand le singe arrête de s'amuser, mon "niveau d'inquiétude" est divisé par 3.
Pour la partie 2, il faut simuler 10 000 rounds sans la division du "niveau d'inquiétude"
Le problème de simuler 10 000 rounds sans diviser le "niveau d'inquiétude" fait que les nombres vont vite devenir très grands.
L'astuce pour passer outre cela, étant donné que le test de chaque singe est un nombre premier, est d'utiliser le "Théorème chinois du reste".
Une application de ce théorème permet de réduire un grand nombre tout en conservant sa propriété d'avoir le même reste pour toute division avec un nombre premier:
Prenons le nombre 5000. Nous voulons pouvoir le réduire tout en conservant le même reste si on le divise par 3, 5 et 11.
5000 / 3 = 1666 reste 2
5000 / 5 = 1000 reste 0
5000 / 11 = 454 reste 6
Si on divise 5000 par 165 (3x5x11), le résultat est 30 reste 50.
50 / 3 = 16 reste 2
50 / 5 = 10 reste 0
50 / 11 = 4 reste 6
Nous avons pu réduire notre nombre tout en conservant les mêmes propriétés qui nous intéressent.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector<string> split(string s, string delim)
{
  vector<string> result;
  size_t start = 0, end = 0;
  while ((end = s.find(delim, start)) != string::npos)
  {
    result.push_back(s.substr(start, end - start));
    start = end + delim.length();
  }
  result.push_back(s.substr(start));
  return result;
}

struct Monkey
{
  vector<long long> items;
  char operation;
  string operationValue;
  int test;
  int testTrue;
  int testFalse;
  long long nbInspected = 0;
};

const int nbMonkeys = 8;
int moduloMagique = 1;

Monkey monkeysP1[nbMonkeys];
Monkey monkeysP2[nbMonkeys];

/*
Cette fonction calcule le "niveau d'inquiétude" en fonction de l'opération pour ce singe (passé en paramètre),
puis divise par 3 si on calcule pour la partie 1. Sinon, elle modulo avec le nombre du théorème du reste chinois si nous sommes sur la partie 2.
Elle effectue le test de division et donne l'objet à l'un des singes en fonction du résultat de ce test.
*/
void calculateWorryLevel(long long item, Monkey m, bool p2, Monkey (&monkeys)[nbMonkeys])
{
  int operationValue = m.operationValue == "old" ? item : stoi(m.operationValue);
  switch (m.operation)
  {
  case '+':
    item += operationValue;
    break;
  case '*':
    item *= operationValue;
    break;
  }
  if (!p2)
  {
    item = static_cast<long long>(floor((double)item / 3));
  }
  else
  {
    item = item % moduloMagique;
  }
  if (item % m.test == 0)
  {
    monkeys[m.testTrue].items.push_back(item);
  }
  else
  {
    monkeys[m.testFalse].items.push_back(item);
  }
}
// Cette fonction retourne le produit du "Monkey Business".
long long getMonkeyBusiness(Monkey (&monkeys)[nbMonkeys])
{
  long long max1 = 0;
  long long max2 = 0;

  for (int i = 0; i < nbMonkeys; i++)
  {
    Monkey m = monkeys[i];
    if (m.nbInspected > max2)
    {
      max2 = m.nbInspected;
      if (max2 > max1)
      {
        max2 = max1;
        max1 = m.nbInspected;
      }
    }
  }
  return max1 * max2;
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
  int indexMonkey = 0;
  int lineIndex = 0;
  while (getline(file, line))
  {
    if (lineIndex == 0 && !line.empty()) // Initialiser le singe
    {
      monkeysP1[indexMonkey] = Monkey{{}, '+', "0", 1, 0, 0};
      monkeysP2[indexMonkey] = Monkey{{}, '+', "0", 1, 0, 0};
    }
    else if (lineIndex == 1) // Objets de départ
    {
      vector<string> splitedItem = split(line.substr(18), ", ");
      int nbItems = splitedItem.size();
      for (int i = 0; i < nbItems; i++)
      {
        long long item = stoll(splitedItem[i]);
        monkeysP1[indexMonkey].items.push_back(item);
        monkeysP2[indexMonkey].items.push_back(item);
      }
    }
    else if (lineIndex == 2) // Opération
    {
      vector<string> splitedOperation = split(line.substr(23), " ");
      monkeysP1[indexMonkey].operation = splitedOperation[0][0];
      monkeysP1[indexMonkey].operationValue = splitedOperation[1];

      monkeysP2[indexMonkey].operation = splitedOperation[0][0];
      monkeysP2[indexMonkey].operationValue = splitedOperation[1];
    }
    else if (lineIndex == 3) // Test
    {
      int t = stoi(line.substr(21));
      monkeysP1[indexMonkey].test = t;
      monkeysP2[indexMonkey].test = t;
      moduloMagique *= t;
    }
    else if (lineIndex == 4) // Test true
    {
      int t = stoi(line.substr(29));
      monkeysP1[indexMonkey].testTrue = t;
      monkeysP2[indexMonkey].testTrue = t;
    }
    else if (lineIndex == 5) // Test false
    {
      int t = stoi(line.substr(30));
      monkeysP1[indexMonkey].testFalse = t;
      monkeysP2[indexMonkey].testFalse = t;
      indexMonkey++;
      lineIndex = -1;
    }
    if (!line.empty())
    {
      lineIndex++;
    }
  }
  file.close();
  // Partie 1
  for (int _ = 0; _ < 20; _++)
  {
    for (int i = 0; i < nbMonkeys; i++)
    {
      Monkey *m = &monkeysP1[i];
      while (m->items.size() != 0)
      {
        m->nbInspected++;
        long long item = m->items.back();
        m->items.pop_back();
        calculateWorryLevel(item, *m, false, monkeysP1);
      }
    }
  }
  long long partie1 = getMonkeyBusiness(monkeysP1);
  // Partie 2
  for (int _ = 0; _ < 10000; _++)
  {
    for (int i = 0; i < nbMonkeys; i++)
    {
      Monkey *m = &monkeysP2[i];
      while (m->items.size() != 0)
      {
        m->nbInspected++;
        long long item = m->items.back();
        m->items.pop_back();
        calculateWorryLevel(item, *m, true, monkeysP2);
      }
    }
  }
  long long partie2 = getMonkeyBusiness(monkeysP2);
  cout << "Partie 1: " << partie1 << '\n';
  cout << "Partie 2: " << partie2;
  return 0;
}