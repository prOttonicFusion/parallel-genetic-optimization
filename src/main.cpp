/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include "city.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include <algorithm>
#include <iostream>
#include <random>

int main(int argc, char *argv[])
{
  // Parse command line arguments
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <coordFile> <maxIter> [wrtToScreen] [wrtToFile]" << std::endl;
    std::cerr << "      coordFile --- A xyz-file containin the city coordinates" << std::endl;
    std::cerr << "      maxIter ----- The maximum number of generations to allow" << std::endl;
    std::cerr << "      wrtToScreen - Write results to screen every this many iterations. Default = 1" << std::endl;
    std::cerr << "      wrtToFile --- Write results to file every this many iterations. Default = 1" << std::endl;
    return -1;
  }

  std::string inpuFile = argv[1];          // The name/path of the coordinate file
  const int maxIterations = atoi(argv[2]); // Max. number of iterations if solution doesn't converge
  const int popSize = 100;                 // Size of population
  const int eliteSize = 20;                // Number of top-fitness individs to be allowed to breed
  const float replaceProportion = 0.85;    // Fraction of population to be replaced by children every iteration
  const float mutationProbability = 0.1;   // The probability of offspring getting mutated

  const int crossPerIter = (int)(replaceProportion * popSize); //Number of crossowers/iteration

  // Set screen & output file update intervals
  const int writeToScreenInterval = (argc > 3) ? atoi(argv[3]) : 1;
  const int writeToFileInterval = (argc > 4) ? atoi(argv[4]) : 1;

  int Ncities; // Number of cities to use in calculations
  int *route;
  City *cities; // Array containing all citites
  Individ population[popSize];

  // Read city coordinates from input file
  if (!parseXYZFile(inpuFile, Ncities, cities))
  {
    std::cerr << "Error: Could not read coordinate file" << std::endl;
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
    population[i].init(route, cities, Ncities);
  }

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cerr << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  ////////////////////// Main calculation loop //////////////////////
  int iterCount = 0;
  while (iterCount < maxIterations)
  {
    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on route length
    std::sort(std::begin(population), std::end(population));

    // --------------- Write data to screen & file ------------------
    std::string bestRouteStr = population[0].getRouteAsString(cities, Ncities);
    float bestRouteLen = population[0].distance;
    if (writeToScreenInterval)
      if (iterCount % writeToScreenInterval == 0)
        writeToScreen(iterCount, bestRouteStr, bestRouteLen);
    if (writeToFileInterval)
      if (iterCount % writeToFileInterval == 0)
        writeToOutputFile(iterCount, population[0].route, bestRouteStr, bestRouteLen, Ncities);

    // ------------------------ Breeding ----------------------------
    // Pair a fraction of the population; select parents from the most fit
    // and replace those less fit with offspring
    for (int i = 1; i <= crossPerIter; i++)
    {
      int parent1 = uniformRand(rng) * eliteSize;
      int parent2 = uniformRand(rng) * eliteSize;
      parent2 = (parent1 == parent2) ? parent2 + 1 : parent2;

      Individ child = breedIndivids(population[parent1], population[parent2], cities, popSize, Ncities);

      // Mutation
      if (uniformRand(rng) < mutationProbability)
        mutateIndivid(child, Ncities, rng);

      population[popSize - i] = child;
    }

    iterCount++;
  }

  return 0;
}