/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include <algorithm>
#include <iostream>
#include <random>

int main()
{
  const int popSize = 1000;        // Size of population
  const int crossPerIteration = 2; // Number of individs to crossover each iteration

  int Ncities; // Number of cities to use in calculations
  float *xpos; // The x-coordinates of each city
  float *ypos; // The y-coordinates of each city
  int *route;
  std::string *cityNames; // Names of each city
  Individ population[popSize];

  // Read city coordinates from input file
  if (!parseXYZFile(Ncities, cityNames, xpos, ypos))
  {
    std::cout << "Error: Could not read coordinate file" << std::endl;
    return -1;
  }

  // Initialize route array
  route = new int[Ncities];
  for (int i = 0; i < Ncities; i++)
    route[i] = i;

  // -------------- Initialize random number generator --------------
  static std::random_device rd;  // Random seed generator
  static std::mt19937 rng(rd()); // RNG (Mersenne Twister)

  // ----------------- Generate initial population ------------------
  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(&route[0], &route[Ncities - 1], rng); // Shuffle route
    population[i].init(route, xpos, ypos, Ncities);
  }

  ////////////////////// Main calculation loop //////////////////////
  Individ fittest[crossPerIteration];
  bool hasConverged = false;
  while (!hasConverged)
  {
    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on distance
    std::sort(std::begin(population), std::end(population));

    // ----------------------- Selection ----------------------------

    // ----------------------- Crossover ----------------------------

    // ------------------------ Mutation ----------------------------

    // ---------------- Check convergence status --------------------
    hasConverged = true;
  }

  if (!writeToOutputFile("test output", true))
  {
    std::cout << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  return 0;
}