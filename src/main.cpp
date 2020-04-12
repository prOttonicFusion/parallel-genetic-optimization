/********************************************************************
 * Main program
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
  const int globalPopSize = 100;           // Combined size of all populations
  const int eliteSize = 20;                // Number of top-fitness individs to be allowed to breed
  const int eliteMigrationSize = 2;        // How many of the fittest to share with neighbors
  const int eliteMigrationInterval = 100;  // Send fittest individuals to neighbor CPUs every this many iterations
  const float replaceProportion = 0.85;    // Fraction of population to be replaced by children every iteration
  const float mutationProbability = 0.1;   // The probability of offspring getting mutated

  // Set screen & output file update intervals
  const int writeToScreenInterval = (argc > 3) ? atoi(argv[3]) : 1;
  const int writeToFileInterval = (argc > 4) ? atoi(argv[4]) : 1;

  int Ncities; // Number of cities to use in calculations
  int *route;
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
  static std::mt19937 rng(rd()); // RNG (Mersenne Twister)
  static std::uniform_real_distribution<float> uniformRand(0.0, 1.0);

  // -------------------- Initialize output file --------------------
  if (!writeToOutputFile("", true))
  {
    std::cerr << "Error: Problem with writing to ouput file" << std::endl;
    return -1;
  }

  // ------------------------ Initialize MPI ------------------------
  const int tag = 50;
  int Ntasks, rank, nameLenght, dest_id, source_id;
  char procName[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
  {
    std::cerr << "Error: MPI initialization failed" << std::endl;
    return -1;
  }

  // Query process information
  MPI_Comm_size(MPI_COMM_WORLD, &Ntasks);        // Ntasks = number of processes/CPUs
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);          // rank = id of current process
  MPI_Get_processor_name(procName, &nameLenght); // procName = name of current process

  // Per-process population size
  const int popSize = (int)(globalPopSize / Ntasks + 0.5f);
  const int crossPerIter = (int)(replaceProportion * popSize); //Number of crossowers/iteration

  // Map the processes to a 2D grid topology
  int ndims = 2; // Number of dimensions
  //int p = (int)sqrt(Ntasks);   // Number of CPUs per dim (p x p grid)
  int dims[ndims] = {0, 0};    // Number of nodes along each dim (0 --> let MPI_Dims determine )
  int reorder = 0;             // Should MPI determine optimal process ordering?
  int periods[ndims] = {1, 1}; // Periodicity (0 or 1) in each direction?
  MPI_Comm GRID_COMM;
  MPI_Dims_create(Ntasks, ndims, dims);
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &GRID_COMM);

  // ----------------- Generate initial population ------------------
  Individ population[popSize];
  for (int i = 0; i < popSize; i++)
  {
    std::shuffle(&route[0], &route[Ncities - 1], rng); // Shuffle route
    population[i].init(route, cities, Ncities);
  }

  // Sort population in ascending order based on distance
  std::sort(population, population + popSize);

  ////////////////////// Main calculation loop //////////////////////
  int indexToBreed1, indexToBreed2, iterCount = 0;
  bool hasConverged = false;
  while (!hasConverged && iterCount < maxIterations)
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
    // Sort population in ascending order based on distance
    std::sort(population, population + popSize);

    // --------------- Write data to screen & file ------------------
    if (rank == 0)
    {
      // TODO: collect data from the other processes
      std::string bestRouteStr = population[0].getRouteAsString(cities, Ncities);
      float bestRouteLen = population[0].distance;
      if (writeToScreenInterval)
        if (iterCount % writeToScreenInterval == 0)
          writeToScreen(iterCount, bestRouteStr, bestRouteLen);
      if (writeToFileInterval)
        if (iterCount % writeToFileInterval == 0)
          writeToOutputFile(iterCount, population[0].route, bestRouteStr, bestRouteLen, Ncities);
    }

    // ------------ Share data with closest neighbor CPUs -----------
    if (iterCount % eliteMigrationInterval == 0 && iterCount > 0)
    {
      int recvdRoute[Ncities];
      for (int i = 0; i < eliteMigrationSize; i++)
        for (int j = 0; j < 2; j++)
        {
          MPI_Cart_shift(GRID_COMM, j, 1, &source_id, &dest_id);
          MPI_Sendrecv(population[i].route, Ncities, MPI_INT, dest_id, tag, recvdRoute,
                       Ncities, MPI_INT, source_id, tag, GRID_COMM, &status);
          population[popSize - (i * 2 + 1)].setRoute(recvdRoute, cities, Ncities);

          MPI_Cart_shift(GRID_COMM, j, -1, &source_id, &dest_id);
          MPI_Sendrecv(population[i].route, Ncities, MPI_INT, dest_id, tag, recvdRoute,
                       Ncities, MPI_INT, source_id, tag, GRID_COMM, &status);
          population[popSize - (i * 2 + 2)].setRoute(recvdRoute, cities, Ncities);
        }
    }
    iterCount++;
  }

  // Finalize MPI & quit program
  MPI_Finalize();
  return 0;
}