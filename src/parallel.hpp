/********************************************************************
 * @file      parallel.hpp
 * @author    O. Lindblom
 *
 * Functions related to parallel communication
 *
 *******************************************************************/
#ifndef PARALLEL_H
#define PARALLEL_H

#include "city.hpp"
#include "individ.hpp"
#include "population.hpp"
#include <mpi.h>
#include <vector>

/**
 * Compares the shortest routes on each CPU and determine the globally shortest one
 * On the CPU with rank 0, globalFittest will contain the resulting route
 *
 * @param globalFittest  [in/out] The fittest individual globally
 * @param localFittest  [in] The fittest individual locally
 * @param Ncities  [in] The size of cities
 * @param mpi_City  [in] MPI serializer for City struct
 * @param rank  [in] The id of the current CPU
 * @param Ntasks [in] The total number of CPUs
 * @param tag  [in] MPI messaging tag
 * @param comm  [in] MPI Communicator object
 * @param status [in] MPI_Status
 */
void getGlobalFittestRoute(Individ &globalFittest, const Individ &localFittest, const int &Ncities,
                           MPI_Datatype mpi_City, const int &rank, const int &Ntasks,
                           const int &tag, MPI_Comm &comm, MPI_Status &status);

/**
 * Compares the shortest routes on each CPU and determine the globally shortest one
 * On the CPU with rank 0, globalShortestRouteLength will contain the resulting route length
 * This is a more light-weight version of getGlobalFittestRoute()
 *
 * @param globalShortestRouteLength  [in/out] Length of the globally shortest route
 * @param localFittest  [in] The fittest Individ object on the current CPU
 * @param rank  [in] The id of the current CPU
 * @param Ntasks [in] The total number of CPUs
 * @param comm  [in] MPI Communicator object
 */
void getGlobalFittestRouteLenght(float &globalShortestRouteLength, const Individ &localFittest,
                                 const int &rank, const int &Ntasks, MPI_Comm &comm);
/**
 * Copy over migrationSize random individuals from each CPU to its right-side neighbor in a circular
 * CPU topology
 *
 * @param migrationSize  [in] Number of individuals to migrate from each CPU
 * @param tournamentSize  [in] Argument for selectRandomIndivid()
 * @param population  [in/out] The population, i.e. an array of Individ objects
 * @param populationSize  [in] Size of population
 * @param Ncities  [in] The size of cities
 * @param mpi_City  [in] MPI serializer for City struct
 * @param rank  [in] The id of the current CPU
 * @param Ntasks  [in] The total number of CPUs
 * @param tag  [in] MPI messaging tag
 * @param GRID_COMM  [in] MPI Communicator with processors mapped to a periodic 1D grid topology
 * @param status  [in] MPI_Status
 */
void performMigration(const int &migrationSize, const int &tournamentSize, Population population,
                      const int &Ncities, MPI_Datatype mpi_City, const int &rank, const int &Ntasks,
                      const int &tag, MPI_Comm &GRID_COMM, MPI_Status &status);

#endif