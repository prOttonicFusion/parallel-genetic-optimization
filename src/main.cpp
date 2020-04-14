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
#include <algorithm>
#include <iostream>
#include <mpi.h>
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
  const int globalPopSize = 1000;          // Combined size of all populations
  const int eliteSize = 20;                // Number of top-fitness individs to be allowed to breed
  const int eliteMigrationSize = 2;        // How many of the fittest to share with neighbors
  const int eliteMigrationInterval = 50;   // Send fittest individuals to neighbor CPUs every this many iterations
  const float replaceProportion = 0.85;    // Fraction of population to be replaced by children every iteration
  const float mutationProbability = 0.1;   // The probability of offspring getting mutated

  // Set screen & output file update intervals
  const int writeToScreenInterval = (argc > 3) ? atoi(argv[3]) : 1;
  const int writeToFileInterval = (argc > 4) ? atoi(argv[4]) : 1;

  int Ncities;  // Number of cities to use in calculations
  int *route;   // Array containing city indices in a specific order
  City *cities; // Array containing all citites

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
  static std::mt19937 rng(rd()); // Random engine (Mersenne Twister)
  static std::uniform_real_distribution<float> uniformRand(0.0, 1.0);

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cerr << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  // ------------------------ Initialize MPI ------------------------
  const int tag = 50;
  int Ntasks, rank, nameLenght, destRank, sourceRank;
  char procName[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
  {
    std::cerr << "Error: MPI initialization failed" << std::endl;
    return -1;
  }

  // Query process information
  MPI_Comm_size(MPI_COMM_WORLD, &Ntasks); // Ntasks = number of processes/CPUs
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // rank = id of current process
  //MPI_Get_processor_name(procName, &nameLenght); // procName = name of current process

  // Per-process population size
  const int popSize = (int)(globalPopSize / Ntasks + 0.5f);
  const int crossPerIter = (int)(replaceProportion * popSize); //Number of crossowers/iteration

  // Map the processors to a 2D grid topology for more structured communication
  int ndims = 2;                        // Number of dimensions
  int dims[ndims] = {0, 0};             // Number of nodes along each dim (0 --> let MPI_Dims choose)
  int reorder = 0;                      // Should MPI determine optimal process ordering?
  int periods[ndims] = {1, 1};          // Periodicity in each direction; 0 --> non-periodic
  MPI_Comm GRID_COMM;                   // New communicator
  MPI_Dims_create(Ntasks, ndims, dims); // Divide processors in a cartesian grid
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &GRID_COMM);

  // ----------------- Generate initial population ------------------
  Individ population[popSize];
  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(&route[0], &route[Ncities - 1], rng); // Shuffle route
    population[i].init(route, cities, Ncities);
  }

  // Sort population in ascending order based on route lenght
  std::sort(population, population + popSize);

  ////////////////////// Main calculation loop //////////////////////
  int iterCount = 0;
  while (iterCount < maxIterations)
  {
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

    // --------------------- Compute fitness ------------------------
    // Sort population in ascending order based on route length
    std::sort(population, population + popSize);

    // --------------- Write data to screen & file ------------------
    // TODO: gather the best routes from each process & print the globally best route
    if (writeToScreenInterval)
      if (iterCount % writeToScreenInterval == 0)
      {
        float bestRouteLen = population[0].routeLength;
        float globalFittestLengths[Ntasks]; // The lengths of each process' fittest individual
        // Gather the lenghts of the best routes from all CPUs to CPU 0
        MPI_Gather(&bestRouteLen, 1, MPI_FLOAT, globalFittestLengths, 1, MPI_FLOAT, 0, GRID_COMM);

        if (rank == 0)
        {
          // Find globally shortest route
          std::sort(globalFittestLengths, globalFittestLengths + Ntasks);
          writeToScreen(iterCount, globalFittestLengths[0]);
        }
      }

    if (writeToFileInterval)
      if (iterCount % writeToFileInterval == 0)
      {
        int bestRoute[Ncities];
        for (int i = 0; i < Ncities; i++)
          bestRoute[i] = population[0].route[i];
        float bestRouteLen = population[0].routeLength;
        float globalFittestLengths[Ntasks]; // The lengths of each process' fittest individual
        MPI_Allgather(&bestRouteLen, 1, MPI_INT, globalFittestLengths, 1, MPI_INT, GRID_COMM);
        std::sort(globalFittestLengths, globalFittestLengths + Ntasks);
        std::cout << globalFittestLengths[0] << " " << globalFittestLengths[3] << std::endl;

        // Send shortest route
        if (bestRouteLen == globalFittestLengths[0])
          MPI_Bcast(bestRoute, Ncities, MPI_INT, rank, GRID_COMM);

        if (rank == 0)
        {
          std::string bestRouteStr = getRouteAsString(bestRoute, cities, Ncities);
          writeToOutputFile(iterCount, bestRoute, bestRouteStr, bestRouteLen, Ncities);
        }
      }

    // ------------ Share data with closest neighbor CPUs -----------
    if (iterCount % eliteMigrationInterval == 0 && iterCount > 0)
    {
      int recvdRoute[Ncities];
      for (int i = 0; i < eliteMigrationSize; i++)
        for (int j = 0; j < 2; j++)
        {
          // Right & bottom neighbors:
          // Get send & receive adresses for closest neighbor communication
          MPI_Cart_shift(GRID_COMM, j, 1, &sourceRank, &destRank);
          // Send & receive fittest individuals
          MPI_Sendrecv(population[i].route, Ncities, MPI_INT, destRank, tag, recvdRoute,
                       Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
          // Add route received from neighbor to own population
          population[popSize - (i * 2 + 1)].setRoute(recvdRoute, cities, Ncities);

          // Left & top neighbors:
          MPI_Cart_shift(GRID_COMM, j, -1, &sourceRank, &destRank);
          MPI_Sendrecv(population[i].route, Ncities, MPI_INT, destRank, tag, recvdRoute,
                       Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
          population[popSize - (i * 2 + 2)].setRoute(recvdRoute, cities, Ncities);
        }
    }
    iterCount++;
  }

  // Finalize MPI & quit program
  MPI_Finalize();
  return 0;
}