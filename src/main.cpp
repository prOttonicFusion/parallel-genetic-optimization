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
  // Parse command line arguments
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <coordFile> <maxIter>" << std::endl;
    std::cerr << "      coordFile --- A xyz-file containin the city coordinates" << std::endl;
    std::cerr << "      maxIter ----- The maximum number of generations to allow" << std::endl;
    return -1;
  }

  std::string inpuFile = argv[1];          // The name/path of the coordinate file
  const int maxIterations = atoi(argv[2]); // Max. number of iterations if solution doesn't converge
  const int popSize = 100;                 // Size of population
  const int fittestSize = 20;              // Number of top-fitness individs to be allowed to breed
  const float replaceProportion = 0.85;    // Proportion of population be replaced by offspring every iteration
  const float mutationProbability = 0.1;   // The probability of offspring getting mutated
  const int writeToScreenInterval = 1;     // Output data to screen every this many iterations
  const int writeToFileInterval = 1;       // Output data to file every this many iterations

  const int crossPerIter = (int)(replaceProportion * popSize); //Number of crossowers/iteration

  int Ncities; // Number of cities to use in calculations
  float *xpos; // The x-coordinates of each city
  float *ypos; // The y-coordinates of each city
  int *route;
  std::string *cityNames; // Names of each city
  Individ population[popSize];

  // Read city coordinates from input file
  if (!parseXYZFile(inpuFile, Ncities, cityNames, xpos, ypos))
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
    population[i].init(route, xpos, ypos, Ncities);
  }

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cerr << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  ////////////////////// Main calculation loop //////////////////////
  int indexToBreed1, indexToBreed2, iterCount = 0;
  bool hasConverged = false;
  while (!hasConverged && iterCount < maxIterations)
  {
    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on distance
    std::sort(std::begin(population), std::end(population));

    // --------------- Write data to screen & file ------------------
    std::string bestRouteStr = population[0].getRouteAsString(cityNames, Ncities);
    float bestRouteLen = population[0].distance;
    if (iterCount % writeToScreenInterval == 0)
      writeToScreen(iterCount, bestRouteStr, bestRouteLen);
    if (iterCount % writeToFileInterval == 0)
      writeToOutputFile(iterCount, population[0].route, bestRouteStr, bestRouteLen, Ncities);

    // ------------------------ Breeding ----------------------------
    // Pair a fraction of the population; select parents from the most fit
    // and replace those less fit with offspring
    for (int i = 1; i <= crossPerIter; i++)
    {
      int parent1 = uniformRand(rng) * fittestSize;
      int parent2 = uniformRand(rng) * fittestSize;
      parent2 = (parent1 == parent2) ? parent2 + 1 : parent2;

      Individ child = breedIndivids(population[parent1], population[parent2], xpos, ypos, popSize, Ncities);

      // Mutation
      if (uniformRand(rng) < mutationProbability)
        mutateIndivid(child, Ncities, rng);

      population[popSize - i] = child;
    }

    // ---------------- Check convergence status --------------------

    iterCount++;
  }

  return 0;
}