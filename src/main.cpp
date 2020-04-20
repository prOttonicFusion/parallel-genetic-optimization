/********************************************************************
 * @file      main.cpp
 * @author    O. Lindblom
 *
 * The main program
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "IO.hpp"
#include "city.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include "parallel.hpp"
#include "random.hpp"
#include <algorithm>
#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char *argv[])
{
  // Parse command line arguments
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <coordFile> <maxGen> [wrtToScreen] [wrtToFile]"
              << std::endl;
    std::cerr << "  coordFile:   A xyz-file containing the city coordinates" << std::endl;
    std::cerr << "  maxGen:     The maximum number of generations to allow" << std::endl;
    std::cerr << "  wrtToScreen: Write results to screen every this many iterations. Default: 1"
              << std::endl;
    std::cerr << "  wrtToFile:   Write results to file every this many iterations. Default: 1"
              << std::endl;
    return -1;
  }

  std::string inputFile = argv[1];       // The path of the coordinate file
  const int maxGenCount = atoi(argv[2]); // Max. number of generation
  const int globalPopSize = 1000;        // Combined size of all populations
  const float eliteFraction = 0.02;      // Fraction of population conserved to next generation
  const int migrationSize = 2;           // Number of individuals to share with neighbor CPUs
  const int migrationPeriod = 20;        // Send fittest individuals to neighbor
                                         //  CPUs every this many generations
  const float mutationProbability = 0.1; // Probability of offspring mutation
  const int tournamentSize = 5;          // The number of individuals to choose new parents from
  const int writeToScreenPeriod =
      (argc > 3) ? atoi(argv[3]) : 1; // Print to screen every this many generations
  const int writeToFilePeriod =
      (argc > 4) ? atoi(argv[4]) : 1; // Print to file every this many generations

  int Ncities;              // Number of cities to use in calculations
  std::vector<int> route;   // Array containing city indices in a specific order
  std::vector<City> cities; // Array containing all citites

  // Read city coordinates from input file
  if (!parseXYZFile(inputFile, Ncities, cities))
  {
    std::cerr << "Error: Unable to read coordinate file '" << inputFile << "'" << std::endl;
    return -1;
  }

  // Initialize route array
  route.resize(Ncities);
  for (int i = 0; i < Ncities; i++)
    route[i] = i;

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cerr << "Error: Unable to write to ouput file" << std::endl;
    return -1;
  }

  // ------------------------ Initialize MPI ------------------------
  const int tag = 50;
  int Ntasks, rank, destRank, sourceRank;
  MPI_Status status;

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
  {
    std::cerr << "Error: MPI initialization failed" << std::endl;
    return -1;
  }

  // Query process information
  MPI_Comm_size(MPI_COMM_WORLD, &Ntasks); // Ntasks = number of processes/CPUs
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // rank = id of current process

  // Map the processors to a 2D grid topology for more structured communication
  int ndims = 2;               // Number of dimensions
  int dims[ndims] = {0, 0};    // Number of nodes along each dim (0 --> let MPI_Dims choose)
  int reorder = 0;             // Should MPI determine optimal process ordering?
  int periods[ndims] = {1, 1}; // Periodicity in each direction; 0 --> non-periodic
  MPI_Comm GRID_COMM;          // New communicator
  MPI_Dims_create(Ntasks, ndims, dims); // Divide processors in a cartesian grid
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &GRID_COMM);

  // ----------------- Generate initial population ------------------
  // Define per-process population size (should be an even number)
  int tmp = (int)(globalPopSize / Ntasks + 0.5f);
  const int popSize = (tmp % 2 == 0) ? tmp : tmp + 1;
  const int eliteSize = (int)(eliteFraction * popSize); // Number of individuals allowed to breed

  Individ population[popSize];
  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(route.begin(), route.end(), rng); // Shuffle route
    population[i].init(route, cities);
  }

  // Sort population in ascending order based on route lenght
  std::sort(population, population + popSize);

  ////////////////////// Main calculation loop //////////////////////
  int generation = 0;
  while (generation < maxGenCount)
  {
    // ------------------------ Breeding ----------------------------
    // Pair a fraction of the population; select parents from the most fit
    // and replace those less fit with offspring
    Individ nextGeneration[popSize];

    for (int i = eliteSize; i < popSize; i++)
    {
      Individ child = population[i];
      int parent1 = selectParent(population, popSize, tournamentSize);
      int parent2 = selectParent(population, popSize, tournamentSize);
      breedIndivids(child, population[parent1], population[parent2], cities, popSize);
      nextGeneration[i] = child;

      // Mutate
      if (uniformRand(rng) < mutationProbability) mutateIndivid(nextGeneration[i]);
    }

    // Replace population with the new generation
    for (int i = eliteSize; i < popSize; i++)
      population[i] = nextGeneration[i];

    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on route length
    std::sort(population, population + popSize);

    // --------------- Write data to screen & file ------------------
    // Gather the best routes from each process & print the globally best route
    if (writeToScreenPeriod != 0)
      if (generation % writeToScreenPeriod == 0)
      {
        float bestRouteLen = population[0].routeLength;
        float globalFittestLengths[Ntasks]; // The lengths of each process' fittest individual
        // Gather the lenghts of the best routes from all CPUs to CPU 0
        MPI_Gather(&bestRouteLen, 1, MPI_FLOAT, globalFittestLengths, 1, MPI_FLOAT, 0, GRID_COMM);

        if (rank == 0)
        {
          // Find globally shortest route & print it to screen
          std::sort(globalFittestLengths, globalFittestLengths + Ntasks);
          writeToScreen(generation, globalFittestLengths[0]);
        }
      }

    if (writeToFilePeriod != 0)
      if (generation % writeToFilePeriod == 0)
      {
        Individ globalFittest;
        getGloballyFittestRoute(globalFittest, population[0], cities, rank, Ntasks, tag, GRID_COMM,
                                status);
        if (rank == 0)
        {
          std::string bestRouteStr = getRouteAsString(globalFittest.route, cities);
          writeToOutputFile(generation, globalFittest.route, bestRouteStr,
                            globalFittest.routeLength);
        }
      }

    // ------------ Share data with closest neighbor CPUs -----------
    if (generation % migrationPeriod == 0 && generation > 0)
    {
      std::vector<int> recvdRoute(Ncities);
      for (int i = 0; i < migrationSize; i++)
        for (int j = 0; j < 2; j++)
        {
          // Right & bottom neighbors:
          // Get send & receive adresses for closest neighbor communication
          MPI_Cart_shift(GRID_COMM, j, 1, &sourceRank, &destRank);
          // Send & receive fittest individuals
          MPI_Sendrecv(population[i].route.data(), Ncities, MPI_INT, destRank, tag,
                       recvdRoute.data(), Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
          // Add route received from neighbor to own population
          population[popSize - (i * 2 + 1)].setRoute(recvdRoute, cities);

          // Left & top neighbors:
          MPI_Cart_shift(GRID_COMM, j, -1, &sourceRank, &destRank);
          MPI_Sendrecv(population[i].route.data(), Ncities, MPI_INT, destRank, tag,
                       recvdRoute.data(), Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
          population[popSize - (i * 2 + 2)].setRoute(recvdRoute, cities);
        }
    }
    generation++;
  }

  ////////////////// Gather & output final results //////////////////
  Individ globalFittest;
  getGloballyFittestRoute(globalFittest, population[0], cities, rank, Ntasks, tag, GRID_COMM,
                          status);

  if (rank == 0)
  {
    // Find globally shortest route & print it to screen
    std::string bestRouteStr = getRouteAsString(globalFittest.route, cities);
    std::cout << "\nFINAL OUTCOME:\n--------------------------------" << std::endl;
    std::cout << "Total number of generations: " << generation << std::endl;
    std::cout << "Length of shortest route:    " << globalFittest.routeLength << std::endl;
    std::cout << std::endl;
    std::cout << "Generated a total of "
              << (generation * (popSize - eliteSize) + eliteSize) * Ntasks << " individual routes"
              << std::endl;
    std::cout << std::endl;

    writeToOutputFile(generation, globalFittest.route, bestRouteStr, globalFittest.routeLength);
  }

  /////////////////// Finalize MPI & quit program ///////////////////
  MPI_Finalize();
  return 0;
}