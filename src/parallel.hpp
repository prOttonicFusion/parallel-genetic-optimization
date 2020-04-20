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
 * Compare the shortest routes on each CPU and determine the globally shortest one
 *
 * @param globalFittest  [in/out] The fittest individual globally 
 * @param population  [in] An array of Individ objects
 * @param cities  [in] An vector of city objects
 * @param rank  [in] The id of the current CPU
 * @param Ntaskks [in] The total number of CPUs
 * @param tag  [in] MPI messaging tag
 * @param comm  [in] MPI Communicator
 * @param statys [in] MPI_Status
 */
void getGloballyFittestRoute(Individ &globalFittest, Individ population[], std::vector<City> cities,
                             int rank, int Ntasks, int tag, MPI_Comm comm, MPI_Status &status);

#endif