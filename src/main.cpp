/********************************************************************
 * @file      main.cpp
 * @author    O. Lindblom
 *
 * The main file of the Parallel Genetic Algorithm TSP solver
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
#include "population.hpp"
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
  std::vector<City> cities;  // Array containing all citites
  Individ globalFittest;     // The most fit individual of the combined populations

  // ------------------------ Initialize MPI ------------------------
  const int tag = 50;
  int Ntasks, rank;
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

  // Create a MPI serializer for the City struct
  int blocklengths[3] = {1, 1, 1};
  MPI_Datatype types[3] = {MPI_INT, MPI_FLOAT, MPI_FLOAT};
  MPI_Datatype mpi_City;
  MPI_Aint offsets[3];

  offsets[0] = offsetof(City, id);
  offsets[1] = offsetof(City, xpos);
  offsets[2] = offsetof(City, ypos);

  MPI_Type_create_struct(3, blocklengths, offsets, types, &mpi_City);
  MPI_Type_commit(&mpi_City);

  // ------------ Parse input on root CPU & broadcast it ------------
  // Parse command line arguments
  if (argc < 3 && rank == 0)
  {
    printUsageInfo(argv[0]);
    MPI_Finalize();
    return -1;
  }

  coordFile = argv[1];
  maxGenCount = atoi(argv[2]);
  writeToScreenPeriod = (argc > 3) ? atoi(argv[3]) : 1;
  writeToFilePeriod = (argc > 4) ? atoi(argv[4]) : 1;

  if (rank == 0)
  {
    if (!parseInputFile(populationSize, eliteFraction, migrationSize, migrationPeriod,
                        mutationProbability, tournamentSize))
    {
      std::cerr << "Error: Unable to read input file" << std::endl;
      MPI_Finalize();
      return -1;
    }

    if (!parseXYZFile(coordFile, Ncities, cities))
    {
      std::cerr << "Error: Unable to read coordinate file '" << coordFile << "'" << std::endl;
      MPI_Finalize();
      return -1;
    }
  }

  MPI_Bcast(&populationSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&migrationSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&migrationPeriod, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&eliteFraction, 1, MPI_FLOAT, 0, GRID_COMM);
  MPI_Bcast(&tournamentSize, 1, MPI_INT, 0, GRID_COMM);
  MPI_Bcast(&mutationProbability, 1, MPI_FLOAT, 0, GRID_COMM);
  MPI_Bcast(&Ncities, 1, MPI_INT, 0, GRID_COMM);
  cities.resize(Ncities);
  MPI_Bcast(cities.data(), Ncities, mpi_City, 0, GRID_COMM);

  // -------------------- Initialize output file --------------------
  if (rank == 0)
    if (!writeToOutputFile("", true))
    {
      std::cerr << "Error: Unable to write to ouput file" << std::endl;
      MPI_Finalize();
      return -1;
    }

  // ----------------- Generate initial population ------------------
  int eliteSize = (int)(eliteFraction * populationSize); // Number of individuals allowed to breed
  Population population(populationSize);
  population.init(cities, Ncities);
  population.sort(); // Sort in ascending order based on route lenght

  ////////////////////// Main calculation loop //////////////////////
  int generation = 0;
  while (generation < maxGenCount)
  {
    // --------------- Write data to screen & file ------------------
    // Gather the best routes from each process & print the globally best route
    if (writeToFilePeriod != 0)
      if (generation % writeToFilePeriod == 0)
      {
        getGlobalFittestRoute(globalFittest, population.individuals[0], cities, rank, Ntasks, tag,
                              GRID_COMM, status);
        if (rank == 0) writeToOutputFile(generation, globalFittest, cities);
      }

    if (writeToScreenPeriod != 0)
      if (generation % writeToScreenPeriod == 0)
      {
        float globalFittestLength = globalFittest.routeLength;
        // If we have written to file this gen., there is no need to recalculate globalFittest
        if (generation % writeToFilePeriod != 0)
          getGlobalFittestRouteLenght(globalFittestLength, population.individuals[0], rank, Ntasks,
                                      GRID_COMM);

        if (rank == 0) writeToScreen(generation, globalFittestLength);
      }

    // ------------------------ Breeding ----------------------------
    // Create new generation from elite + offspring of selected parents
    Individ nextGeneration[populationSize];

    for (int i = eliteSize; i < populationSize; i++)
    {
      Individ parent1 = population.selectRandomIndivid(tournamentSize);
      Individ parent2 = population.selectRandomIndivid(tournamentSize);
      Individ child = population.individuals[i]; // Re-use individ object
      breedIndivids(child, parent1, parent2, cities);
      nextGeneration[i] = child;

      // Mutate
      if (uniformRand(rng) < mutationProbability) mutateIndivid(nextGeneration[i], cities);
    }

    // Replace population with the new generation, leaving elite in place
    for (int i = eliteSize; i < populationSize; i++)
      population.individuals[i] = nextGeneration[i];

    // --------------------- Sort population ------------------------
    // Sort population in ascending order based on (squared) route length
    population.sort();

    // ---------------- Share data with neighbor CPU ----------------
    // Send random individual(s) to closest CPU on the right
    if (generation % migrationPeriod == 0 && generation > 0)
    {
      performMigration(migrationSize, tournamentSize, population, cities, Ncities, rank, Ntasks,
                       tag, GRID_COMM, status);
      population.sort();
    }

    // Increment generation counter
    generation++;
  }

  ////////////////// Gather & output final results //////////////////
  getGlobalFittestRoute(globalFittest, population.individuals[0], cities, rank, Ntasks, tag,
                        GRID_COMM, status);
  if (rank == 0)
  {
    std::cout << "\nFINAL OUTCOME:\n--------------------------------"
              << "\nTotal number of generations: " << generation
              << "\nLength of shortest route:    " << globalFittest.routeLength
              << "\n\nGenerated a total of "
              << (generation * (populationSize - eliteSize) + eliteSize) * Ntasks
              << " individual routes\n"
              << std::endl;
    writeToOutputFile(generation, globalFittest, cities);
  }

  /////////////////// Finalize MPI & quit program ///////////////////
  MPI_Finalize();
  return 0;
}