/********************************************************************
 * @file      parallel.hpp
 * @author    O. Lindblom
 *
 * Header file containing functions related to parallel communication
 *
 * @date      2020-04-20
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/
#ifndef PARALLEL_H
#define PARALLEL_H

#include "individ.hpp"
#include <mpi.h>
#include <vector>

/**
 * Compares the shortest routes on each CPU and determine the globally shortest one.
 * The output is the globally fittest Individ object
 *
 * @param globalFittest  [in/out] The fittest individual globally
 * @param localFittest  [in] The fittest individual locally
 * @param cities  [in] An vector of city objects
 * @param rank  [in] The id of the current CPU
 * @param Ntasks [in] The total number of CPUs
 * @param tag  [in] MPI messaging tag
 * @param comm  [in] MPI Communicator object
 * @param status [in] MPI_Status
 */
void getGlobalFittestRoute(Individ &globalFittest, const Individ &localFittest,
                           const std::vector<City> &cities, const int &rank, const int &Ntasks,
                           const int &tag, MPI_Comm &comm, MPI_Status &status);

/**
 * Compares the shortest routes on each CPU and determine the globally shortest one.
 * The output is the length of globally fittest Individ objects route.
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
 * Copy over migrationSize random individuals from each CPU to its right-side neighbor in a circular CPU topology
 * 
 * @param migrationSize  [in] Number of individuals to migrate from each CPU
 * @param tournamentSize  [in] Argument for selectRandomIndivid()
 * @param population  [in/out] The population, i.e. an array of Individ objects
 * @param populationSize  [in] Size of population
 * @param cities  [in] The cities as City struct array
 * @param Ncities  [in] The size of cities
 * @param rank  [in] The id of the current CPU
 * @param Ntasks  [in] The total number of CPUs
 * @param tag  [in] MPI messaging tag
 * @param GRID_COMM  [in] MPI Communicator with processors mapped to a periodic 1D grid topology
 * @param status  [in] MPI_Status
 */
void performMigration(const int &migrationSize, const int &tournamentSize, Individ population[],
                      const int &populationSize, const std::vector<City> &cities,
                      const int &Ncities, const int &rank, const int &Ntasks, const int &tag,
                      MPI_Comm &GRID_COMM, MPI_Status &status);

#endif