/*
  L'entrée représente l'ancien mot de passe du Père Noël qui a expiré.

  De nouvelles contraintes ont été mises en place pour avoir un mot de passe "robuste":
  - Le mot de passe doit contenir au moins une suite croissante de trois lettres.
  - Les lettres "i", "o" et "l" sont interdites.
  - Le mot de passe doit contenir au moins 2 paires de lettres qui ne se chevauchent pas.

  Pour la partie 1, il faut retrouver le mot de passe suivant en l'incrémentant et qu'il respecte les nouvelles
  contraintes.
  L'incrémentation du mot de passe revient à le considérer comme des nombres : aay --> aaz --> aba --> etc.

  Pour la partie 2, le nouveau mot de passe a encore expiré, il faut encore le trouver.
*/
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include <Utils/MeasureExecutionTime.cpp>

using namespace std;

const int BASE = 26;
const int PASSWORD_SIZE = 8;

string input;

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
    getline(file, input);
    file.close();
    return 0;
}

// Convertit le mot de passe en nombre décimal en considérant que celui-ci est en base 26.
long long passwordToNumber(const string &s)
{
    long long n = 0;
    for (int i = 0; i < PASSWORD_SIZE; i++)
    {
        int charValue = s[i] - 'a';
        n += charValue * pow(BASE, PASSWORD_SIZE - i - 1);
    }
    return n;
}
// Convertit un nombre décimal en mot de passe en considérant que le mot de passe est en base 26.
// Le mot de passe résultant est ajusté à la taille PASSWORD_SIZE.
string numberToPassword(long long n)
{
    string s = "";
    while (n != 0)
    {
        int remainder = n % BASE;
        n = (n - remainder) / BASE;
        s += 'a' + (remainder);
    }
    while (s.size() != PASSWORD_SIZE)
    {
        s += 'a';
    }
    reverse(s.begin(), s.end());

    return s;
}
// Vérifie la validité d'un mot de passe selon les critères spécifiés.
bool passwordIsValid(string &s)
{
    bool valid = false;
    int countPair = 0;
    int lastCharInPair = -2;

    // Parcours du mot de passe
    for (int i = 0; i < PASSWORD_SIZE; i++)
    {
        // Vérification de la présence d'une suite croissante de trois lettres
        if (i < PASSWORD_SIZE - 2)
        {
            if (s[i] + 2 == s[i + 1] + 1 && s[i + 2] == s[i] + 2)
            {
                valid = true;
            }
        }
        // Vérification de la présence d'au moins 2 paires de lettres qui ne se chevauchent pas
        if (i < PASSWORD_SIZE - 1 && lastCharInPair != i - 1 && s[i] == s[i + 1])
        {
            countPair++;
            lastCharInPair = i;
        }
        // Vérification de l'interdiction des lettres 'i', 'l' et 'o'
        if (s[i] == 'i' || s[i] == 'l' || s[i] == 'o')
        {
            return false;
        }
    }
    // Le mot de passe est valide s'il satisfait toutes les conditions
    return valid && countPair >= 2;
}
// Génère un nouveau mot de passe conforme à partir d'un mot de passe initial non valide.
string process(string s)
{
    MEASURE_FUNCTION_EXECUTION_TIME
    long long value = passwordToNumber(s);

    // Tant que le mot de passe n'est pas valide, on incrémente la valeur et on génère un nouveau mot de passe
    while (!passwordIsValid(s))
    {
        value++;
        s = numberToPassword(value);
    }

    return s;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    string part1 = process(input);

    string s = numberToPassword(passwordToNumber(part1) + 1);

    string part2 = process(s);

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}