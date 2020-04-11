/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include <algorithm>
#include <iostream>
#include <random>

int main(int argc, char *argv[])
{
  const int popSize = 50;                 // Size of population
  const int crossPerIteration = 2;        // Number of individs to crossover each iteration
  const int maxIterations = 1000000;      // Max. number of iterations if solution doesn't converge
  const float mutationProbability = 0.05; // The probability of offspring getting mutated
  const int writeToScreenInterval = 100;  // Output data to screen every this many iterations
  const int writeToFileInterval = 100;    // Output data to file every this many iterations

  // Parse command line arguments
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <coordinate-file>" << std::endl;
    return -1;
  }

  std::string inpuFile = argv[1];

  int Ncities; // Number of cities to use in calculations
  float *xpos; // The x-coordinates of each city
  float *ypos; // The y-coordinates of each city
  int *route;
  std::string *cityNames; // Names of each city
  Individ population[popSize];

  // Read city coordinates from input file
  if (!parseXYZFile(inpuFile, Ncities, cityNames, xpos, ypos))
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
  static std::uniform_real_distribution<float> uniformRand(0.0, 1.0);

  // ----------------- Generate initial population ------------------
  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(&route[0], &route[Ncities - 1], rng); // Shuffle route
    population[i].init(route, xpos, ypos, Ncities);
  }

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cout << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  ////////////////////// Main calculation loop //////////////////////
  Individ fittest[crossPerIteration];
  int indexToBreed1, indexToBreed2, iterCount = 0;
  bool hasConverged = false;
  while (!hasConverged && iterCount < maxIterations)
  {
    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on distance
    std::sort(std::begin(population), std::end(population));
    //std::cout << "Best: " << population[0].distance << std::endl;
    //std::cout << "Worst: " << population[popSize-1].distance << std::endl;

    // --------------- Write data to screen & file ------------------
    std::string bestRouteStr = population[0].getRouteAsString(cityNames, Ncities);
    float bestRouteLen = population[0].distance;
    if (iterCount % writeToScreenInterval == 0)
      writeToScreen(iterCount, bestRouteStr, bestRouteLen);
    if (iterCount % writeToFileInterval == 0)
      writeToOutputFile(iterCount, population[0].route, bestRouteStr, bestRouteLen, Ncities);

    // ----------------------- Selection ----------------------------
    // We choose the two fittest individs for breeding
    indexToBreed1 = 0;
    indexToBreed2 = 1;

    // ------------------------ Breeding ----------------------------
    // Pair two fittest individs
    Individ child = breedIndivids(population[0], population[1], xpos, ypos, popSize, Ncities);
    // Replace the 2 least fit individs in population with the new offspring
    population[popSize - 1] = child;
    //population[popSize - 2].setRoute(childRoutes[1], xpos, ypos, Ncities);

    // ------------------------ Mutation ----------------------------
    if (uniformRand(rng) < mutationProbability)
    {
      // Mutate last individ in population (latest offspring)
      mutateIndivid(popSize - 1, population, Ncities, rng);
    }

    // ---------------- Check convergence status --------------------

    iterCount++;
  }

  return 0;
}