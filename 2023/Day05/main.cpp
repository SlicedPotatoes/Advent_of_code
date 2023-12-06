/*

    L'entrée répertorie toutes les graines à planter et contient également une liste de map décrivant la conversion
    des chiffres d'une catégorie source en chiffres d'une catégorie de destination.

    Par exemple, la section débutant par "seed-to-soil map:" explique comment convertir un numéro de graine (source)
    en numéro de sol (destination). Chaque ligne de la map contient trois chiffres: le début de la plage de destination,
    le début de la plage source, et la longueur de la plage.

    Prenons l'exemple de la map "seed-to-soil map:" suivante:
    50 98 2
    52 50 48
    La première ligne indique que la plage source commence à 98 et contient deux valeurs (98, 99).
    La plage de destination a la même longueur mais commence par 50 (50, 51).
    Ainsi, si l'on convertit la graine 99 avec cette map, on obtiendrait 51 en sortie.

    Si un numéro source ne se trouve pas dans les plages de la map, sa destination correspond au même numéro que la
    source. Dans l'exemple précédent, la graine 10 correspondrait au numéro de sol 10.

    L'entrée contient un total de 7 maps:
    - Seed --> Soil
    - Soil --> Fertilizer
    - Fertilizer --> Water
    - Water --> Light
    - Light --> Temperature
    - Temperature --> Humidity
    - Humidity --> Location

    Pour la partie 1, l'objectif est de trouver la localisation minimale par rapport à toutes les graines.

    Pour la partie 2, les graines sont considérées comme des plages par paire.
    Étant donné que la liste des graines a une longueur paire, chaque paire de deux graines représente une plage
    similaire aux plages dans les maps.
    L'objectif reste le même que pour la partie 1, trouver la localisation minimale par rapport à toutes les graines.

*/

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include <Utils/MeasureExecutionTime.cpp>
#include <Utils/SplitString.cpp>

using namespace std;

struct Range
{
    uint32_t destinationStart; // Début de la plage de destination
    uint32_t sourceStart;      // Début de la plage source
    uint32_t rangeLength;      // Longueur de la plage

    bool operator<(const Range &other) const
    {
        return destinationStart < other.destinationStart;
    }
};

vector<uint32_t> seeds;              // Numéros des graines à planter
vector<Range> seedToSoil;            // Conversion graines vers sol
vector<Range> soilToFertilizer;      // Conversion sol vers engrais
vector<Range> fertilizerToWater;     // Conversion engrais vers eau
vector<Range> waterToLight;          // Conversion eau vers lumière
vector<Range> lightToTemperature;    // Conversion lumière vers température
vector<Range> temperatureToHumidity; // Conversion température vers humidité
vector<Range> humidityToLocation;    // Conversion humidité vers emplacement

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

    int step = 0;

    while (getline(file, line))
    {
        if (line == "")
        {
            step++;
            getline(file, line);
            continue;
        }

        if (step == 0)
        {
            line = splitString(line, ": ")[1];
            vector<string> splitedInput = splitString(line, " ");
            for (string s : splitedInput)
            {
                seeds.push_back(stoul(s));
            }
            continue;
        }

        vector<string> splitedInput = splitString(line, " ");
        Range r = {stoul(splitedInput[0]), stoul(splitedInput[1]), stoul(splitedInput[2])};

        switch (step)
        {
        case 1:
            seedToSoil.push_back(r);
            break;
        case 2:
            soilToFertilizer.push_back(r);
            break;
        case 3:
            fertilizerToWater.push_back(r);
            break;
        case 4:
            waterToLight.push_back(r);
            break;
        case 5:
            lightToTemperature.push_back(r);
            break;
        case 6:
            temperatureToHumidity.push_back(r);
            break;
        case 7:
            humidityToLocation.push_back(r);
            break;
        }
    }
    file.close();

    sort(seedToSoil.begin(), seedToSoil.end());
    sort(soilToFertilizer.begin(), soilToFertilizer.end());
    sort(fertilizerToWater.begin(), fertilizerToWater.end());
    sort(waterToLight.begin(), waterToLight.end());
    sort(lightToTemperature.begin(), lightToTemperature.end());
    sort(temperatureToHumidity.begin(), temperatureToHumidity.end());
    sort(humidityToLocation.begin(), humidityToLocation.end());

    return 0;
}

// Retourne la destination en fonction de la source
uint32_t processVector(uint32_t source, vector<Range> &ranges)
{
    for (Range &r : ranges)
    {
        // La source se trouve dans une plage du vecteur
        if (source >= r.sourceStart && source < (r.sourceStart + r.rangeLength))
        {
            return r.destinationStart + source - r.sourceStart;
        }
    }

    return source;
}

// Effectue une série de conversions en utilisant la fonction "processVector" avec les vecteurs de plages spécifiques,
// et retourne finalement la localisation résultante.
uint32_t processSeed(uint32_t seed)
{
    uint32_t soil = processVector(seed, seedToSoil);
    uint32_t fertilizer = processVector(soil, soilToFertilizer);
    uint32_t water = processVector(fertilizer, fertilizerToWater);
    uint32_t light = processVector(water, waterToLight);
    uint32_t temperature = processVector(light, lightToTemperature);
    uint32_t humidity = processVector(temperature, temperatureToHumidity);
    uint32_t location = processVector(humidity, humidityToLocation);

    return location;
}

uint32_t processP1()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    uint32_t lowestLocation = UINT32_MAX;

    for (uint32_t seed : seeds)
    {
        uint32_t value = processSeed(seed);
        if (value < lowestLocation)
        {
            lowestLocation = value;
        }
    }

    return lowestLocation;
}

// Retourne la source en fonction de la destination
uint32_t processVectorReverse(uint32_t source, vector<Range> &ranges)
{
    for (Range &r : ranges)
    {
        // La destination se trouve dans une plage du vecteur
        if (source >= r.destinationStart && source < (r.destinationStart + r.rangeLength))
        {
            return r.sourceStart + source - r.destinationStart;
        }
    }

    return source;
}

// Parcourt les éléments de "seeds" par paires, si le numéro de graine se trouve dans l'une des plages il est valide
bool isValidSeed(uint32_t seed)
{
    for (int i = 0; i < seeds.size() - 1; i += 2)
    {
        int startSeed = seeds[i];
        int endSeed = seeds[i + 1] + startSeed;

        if (seed >= startSeed && seed < endSeed)
        {
            return true;
        }
    }
    return false;
}

// Effectue une série de conversions en utilisant la fonction "processVectorReverse" avec les vecteurs de plages
// spécifiques, et retourne finalement la graine résultante.
uint32_t processSeedReverse(uint32_t location)
{
    uint32_t humidity = processVectorReverse(location, humidityToLocation);
    uint32_t temperature = processVectorReverse(humidity, temperatureToHumidity);
    uint32_t light = processVectorReverse(temperature, lightToTemperature);
    uint32_t water = processVectorReverse(light, waterToLight);
    uint32_t fertilizer = processVectorReverse(water, fertilizerToWater);
    uint32_t soil = processVectorReverse(fertilizer, soilToFertilizer);
    uint32_t seed = processVectorReverse(soil, seedToSoil);

    return seed;
}

uint32_t processP2()
{
    MEASURE_FUNCTION_EXECUTION_TIME

    for (uint32_t i = 0; i < UINT32_MAX; i++)
    {
        uint32_t seed = processSeedReverse(i);

        if (isValidSeed(seed))
        {
            return processSeed(seed);
        }
    }

    return 0;
}

int main()
{
    if (parseInput() == 1)
    {
        return 1;
    }

    uint32_t part1 = processP1();
    uint32_t part2 = processP2();

    cout << "\nPart1: " << part1 << '\n';
    cout << "Part2: " << part2 << '\n';

    return 0;
}