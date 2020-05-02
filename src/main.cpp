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

  std::string coordFile;     // The path of the coordinate file
  int maxGenCount;           // Max. number of generation
  int writeToScreenPeriod;   // Print to screen every this many generations
  int writeToFilePeriod;     // Print to file every this many generations
  int populationSize;        // Combined size of all populations
  float eliteFraction;       // Fraction of population conserved to next generation
  int migrationSize;         // Number of individuals to share with neighbor CPUs
  int migrationPeriod;       // Send fittest individuals to neighbor CPUs every this many gens.
  float mutationProbability; // Probability of offspring mutation
  int tournamentSize;        // The number of individuals to choose new parents from
  int Ncities;               // Number of cities to use in calculations
  std::vector<int> route;    // Array containing city indices in a specific order
  std::vector<City> cities;  // Array containing all citites
  Individ globalFittest;     // The most fit individual of the combined populations

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
  int ndims = 1;              // Number of dimensions
  int dims[ndims] = {Ntasks}; // Number of nodes along each dim
  int reorder = 0;            // Should MPI determine optimal process ordering?
  int periodic[ndims] = {1};  // Periodicity in each direction; 0 --> non-periodic
  MPI_Comm GRID_COMM;         // New communicator
  MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periodic, reorder, &GRID_COMM);

  // ------------ Parse input on root CPU & broadcast it ------------
  if (rank == 0)
  {
    if (!parseInputFile(populationSize, eliteFraction, migrationSize, migrationPeriod,
                        mutationProbability, tournamentSize))
    {
      std::cerr << "Error: Unable to read input file" << std::endl;
      return -1;
    }
  }

  MPI_Bcast(&populationSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&migrationSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&migrationPeriod, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&tournamentSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&eliteFraction, 1, MPI_FLOAT, 0, GRID_COMM);
  MPI_Bcast(&mutationProbability, 1, MPI_FLOAT, 0, GRID_COMM);

  // Parse command line arguments
  if (argc < 3 && rank == 0)
  {
    printUsageInfo(argv[0]);
    return -1;
  }

  coordFile = argv[1];
  maxGenCount = atoi(argv[2]);
  writeToScreenPeriod = (argc > 3) ? atoi(argv[3]) : 1;
  writeToFilePeriod = (argc > 4) ? atoi(argv[4]) : 1;

  // Read city coordinates from input file
  if (!parseXYZFile(coordFile, Ncities, cities))
  {
    std::cerr << "Error: Unable to read coordinate file '" << coordFile << "'" << std::endl;
    return -1;
  }

  // -------------------- Initialize output file --------------------
  if (rank == 0)
    if (!writeToOutputFile("", true))
    {
      std::cerr << "Error: Unable to write to ouput file" << std::endl;
      return -1;
    }

  // ----------------- Generate initial population ------------------
  Individ population[populationSize];
  int eliteSize = (int)(eliteFraction * populationSize); // Number of individuals allowed to breed

  // Initialize route array
  route.resize(Ncities);
  for (int i = 0; i < Ncities; i++)
    route[i] = i;

  for (int i = 0; i < populationSize; i++)
  {
    std::shuffle(route.begin(), route.end(), rng); // New random route by shufflinge cities
    population[i].init(route, cities);
  }

  std::sort(population, population + populationSize); // Ascending order based on route lenght

  ////////////////////// Main calculation loop //////////////////////
  int generation = 0;
  while (generation < maxGenCount)
  {
    // --------------- Write data to screen & file ------------------
    // Gather the best routes from each process & print the globally best route
    if (writeToFilePeriod != 0)
      if (generation % writeToFilePeriod == 0)
      {
        getGlobalFittestRoute(globalFittest, population[0], cities, rank, Ntasks, tag, GRID_COMM,
                              status);
        if (rank == 0) writeToOutputFile(generation, globalFittest, cities);
      }

    if (writeToScreenPeriod != 0)
      if (generation % writeToScreenPeriod == 0)
      {
        float globalFittestLength = globalFittest.routeLength;
        // If we have written to file this gen., there is no need to recalculate globalFittest
        if (generation % writeToFilePeriod != 0)
          getGlobalFittestRouteLenght(globalFittestLength, population[0], rank, Ntasks, GRID_COMM);

        if (rank == 0) writeToScreen(generation, globalFittestLength);
      }

    // --------------------- Fitness computation --------------------
    float routeLengthSum = 0;
    for (int i = 0; i < populationSize; i++)
      routeLengthSum += population[i].routeLength;
    

    // ------------------------ Breeding ----------------------------
    // Create new generation from elite + offspring of selected parents
    Individ nextGeneration[populationSize];

    for (int i = eliteSize; i < populationSize; i++)
    {
      Individ child = population[i];
      int parent1 = selectRandomIndivid(population, populationSize, routeLengthSum);
      int parent2 = selectRandomIndivid(population, populationSize, routeLengthSum);
      breedIndivids(child, population[parent1], population[parent2], cities, populationSize);
      nextGeneration[i] = child;

      // Mutate
      if (uniformRand(rng) < mutationProbability) mutateIndivid(nextGeneration[i]);
    }

    // Replace population with the new generation, leaving elite in place
    for (int i = eliteSize; i < populationSize; i++)
      population[i] = nextGeneration[i];

    // --------------------- Sort population ------------------------
    // Sort population in ascending order based on (squared) route length
    std::sort(population, population + populationSize);

    // ---------------- Share data with neighbor CPU ----------------
    // Send fittest individual(s) to next CPU on the right
    if (generation % migrationPeriod == 0 && generation > 0)
    {
      std::vector<int> recvdRoute(Ncities);
      float recvdRouteLength;

      // Get receiver rank for closest neighbor communication
      MPI_Cart_shift(GRID_COMM, 0, 1, &sourceRank, &destRank);

      for (int i = 0; i < migrationSize; i++)
      {
        // Select random individual to send
        int index = selectRandomIndivid(population, populationSize, routeLengthSum);

        // Send & receive fittest individuals
        MPI_Sendrecv(population[index].route.data(), Ncities, MPI_INT, destRank, tag,
                     recvdRoute.data(), Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
        MPI_Sendrecv(&population[index].routeLength, 1, MPI_FLOAT, destRank, tag, &recvdRouteLength,
                     1, MPI_FLOAT, sourceRank, tag, GRID_COMM, &status);
        // Add route received from neighbor to own population by replacing own least fit indviduals
        // Skip if own least fit are more fit than the new candidates
        if (recvdRouteLength < population[populationSize - (i + 1)].routeLength)
          population[populationSize - (i + 1)].setRoute(recvdRoute, cities);
      }
      std::sort(population, population + populationSize);
    }
    generation++;
  }

  ////////////////// Gather & output final results //////////////////
  getGlobalFittestRoute(globalFittest, population[0], cities, rank, Ntasks, tag, GRID_COMM, status);

  if (rank == 0)
  {
    std::string bestRouteStr = getRouteAsString(globalFittest.route, cities);
    std::cout << "\nFINAL OUTCOME:\n--------------------------------" << std::endl;
    std::cout << "Total number of generations: " << generation << std::endl;
    std::cout << "Length of shortest route:    " << globalFittest.routeLength << std::endl;
    std::cout << std::endl;
    std::cout << "Generated a total of "
              << (generation * (populationSize - eliteSize) + eliteSize) * Ntasks
              << " individual routes" << std::endl;
    std::cout << std::endl;
    writeToOutputFile(generation, globalFittest, cities);
  }

  /////////////////// Finalize MPI & quit program ///////////////////
  MPI_Finalize();
  return 0;
}