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

  /////////////////// Generate initial population ///////////////////

  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(&route[0], &route[Ncities - 1], rng); // Shuffle route
    population[i].init(route, xpos, ypos, Ncities);
  }

  cout << population[0].getRouteAsString(cityNames, Ncities) << endl;
  cout << population[5].getRouteAsString(cityNames, Ncities) << endl;

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