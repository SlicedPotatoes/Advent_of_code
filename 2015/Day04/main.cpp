/*
  L'entrée du puzzle est une chaîne de caractères.

  Dans la première partie, l'objectif est de trouver la plus petite valeur numérique supérieure à 0 à concaténer avec
  l'entrée de manière à ce que le MD5 de cette concaténation commence par 5 zéros.

  Dans la deuxième partie, l'objectif est le même que dans la première, à la différence que le MD5 doit commencer par 6
  zéros.
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>
#include <string>

using namespace std;

string calculateMD5(const string &input)
{
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    MD5_Update(&md5Context, input.c_str(), input.length());

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);

    ostringstream hashStream;
    hashStream << hex << setfill('0');
    for (const auto &byte : result)
    {
        hashStream << setw(2) << hex << static_cast<int>(byte);
    }

    return hashStream.str();
}

bool md5StartByZero(string s, int nbZero)
{
    if (s.size() < nbZero)
    {
        return false;
    }
    for (size_t i = 0; i < nbZero; i++)
    {
        if (s[i] != '0')
        {
            return false;
        }
    }
    return true;
}

int process(string s, int nbZero)
{
    int a = 0;
    string md5 = "";

    while (!md5StartByZero(md5, nbZero))
    {
        a++;
        md5 = calculateMD5(s + to_string(a));
    }

    return a;
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
    getline(file, line);
    file.close();

    cout << "Part1: " << process(line, 5) << '\n';
    cout << "Part2: " << process(line, 6) << '\n';

    return 0;
}