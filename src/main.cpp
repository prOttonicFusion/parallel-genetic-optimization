/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include "genetics.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main()
{
  const int popSize = 1000; // Size of population

  int Ncities; // Number of cities to use in calculationst
  float *xpos; // The x-coordinates of each city
  float *ypos; // The y-coordinates of each city
  int *route;
  std::string *cityNames; // Names of each city
  Individ population[popSize];

  // Read city coordinates from input file
  if (!parseXYZFile(Ncities, cityNames, xpos, ypos))
  {
    cout << "Error: Could not read coordinate file" << endl;
    return -1;
  }

  // Initialize route array
  route = new int[Ncities];
  for (int i = 0; i < Ncities; i++)
    route[i] = i;

  //////////////// Initialize random number generator ///////////////
  // Random seed generator
  static std::random_device rd;

  // Random number generator (Mersenne Twister)
  static std::mt19937 rng(rd());

  // Individ ind1(route, xpos, ypos, Ncities);
  // cout << ind1.getRouteAsString(cityNames, Ncities) << endl;

  /////////////////// Generate initial population ///////////////////
  // Shuffle route
  std::shuffle(&route[0], &route[Ncities], rng);
  std::fill(population, population + popSize, Individ(route, xpos, ypos, Ncities));

  // TODO: Implement fitness function

  // TODO: Implement selection & crossover

  // TODO: Implement mutation

  if (!writeToOutputFile("test output", true))
  {
    cout << "Error: Problem with writing to ouput file" << endl;
    return -1;
  }

  return 0;
}