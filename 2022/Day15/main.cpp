#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cmath>

using namespace std;

struct Point
{
  int x, y;
};

class Input
{
public:
  Point coord;
  Point balise;
  Input(string s)
  {
    regex pattern("x=(-?\\d+), y=(-?\\d+)");
    sregex_iterator it(s.begin(), s.end(), pattern);
    sregex_iterator end;
    int i = 0;
    while (it != end)
    {
      smatch match = *it;
      int x = stoi(match[1]);
      int y = stoi(match[2]);
      it++;
      if (i == 0)
      {
        coord = {x, y};
        i++;
      }
      else
      {
        balise = {x, y};
      }
    }
  }
  void print()
  {
    cout << "X: " << coord.x << " Y: " << coord.y << " Balise: X: " << balise.x << " Y: " << balise.y << "\n";
  }
};

int minX = INT_MAX;
int maxX = INT_MIN;
int minY = INT_MAX;
int maxY = INT_MIN;

int _maxX;
int _maxY;

char **MAP;

void checkCoord(int x, int y)
{
  if (x > maxX)
    maxX = x;
  if (x < minX)
    minX = x;
  if (y > maxY)
    maxY = y;
  if (y < minY)
    minY = y;
}

void initMap()
{
  int _maxX = abs(maxX) + abs(minX) + 1;
  int _maxY = abs(maxY) + abs(minY) + 1;

  MAP = new char *[_maxY];

  for (int y = 0; y < _maxY; y++)
  {
    MAP[y] = new char[_maxX];
    for (int x = 0; x < _maxX; x++)
    {
      MAP[y][x] = '.';
    }
  }
}

int getIndex(int a, bool isX)
{
  if (a < 0)
  {
    return abs(a) + (isX ? minX : minY);
  }
  return a + (isX ? abs(minX) : abs(minY));
}

void showMap()
{
  _maxX = abs(maxX) + abs(minX) + 1;
  _maxY = abs(maxY) + abs(minY) + 1;

  for (int y = 0; y < _maxY; y++)
  {
    for (int x = 0; x < _maxX; x++)
    {
      cout << MAP[y][x];
    }
    cout << "\n";
  }
  cout << "\n";
}

double getDistance(Point p1, Point p2)
{
  int x1 = getIndex(p1.x, true);
  int y1 = getIndex(p1.y, false);
  int x2 = getIndex(p2.x, true);
  int y2 = getIndex(p2.y, false);
  return abs(x1 - x2) + abs(y1 - y2);
}

void sensorCoverage(Point sensor, double distance)
{
  for (int y = 0; y < _maxY; y++)
  {
    for (int x = 0; x < _maxX; x++)
    {
      if (MAP[y][x] == '.' && getDistance(sensor, {x, y}) <= distance)
      {
        MAP[y][x] = '#';
      }
    }
  }
}

int main()
{
  vector<Input> inputs;
  string filename = "D:\\Users\\Kevin\\Desktop\\Repo\\Advent_of_code\\2022\\Day15\\input.txt";
  ifstream file(filename);

  if (!file.is_open())
  {
    cerr << "Erreur : impossible d'ouvrir le fichier " << filename;
    return 1;
  }
  string line;
  while (getline(file, line))
  {
    Input a(line);
    checkCoord(a.coord.x, a.coord.y);
    checkCoord(a.balise.x, a.balise.y);
    inputs.push_back(a);
  }
  file.close();

  initMap();

  showMap();

  for (int i = 0; i < inputs.size(); i++)
  {
    Input in = inputs[i];
    MAP[getIndex(in.coord.y, false)][getIndex(in.coord.x, true)] = 'S';
    MAP[getIndex(in.balise.y, false)][getIndex(in.balise.x, true)] = 'B';

    double d = getDistance(in.coord, in.balise);
    in.print();
    cout << d << "\n";
    Point p = {getIndex(in.coord.x, true), getIndex(in.coord.y, false)};
    sensorCoverage(p, d);
  }
  showMap();

  int count = 0;
  int index = getIndex(2000000, false);
  for (int i = 0; i < _maxX; i++)
  {
    if (MAP[index][i] == '#')
    {
      count++;
    }
  }
  cout << count;
  return 0;
}