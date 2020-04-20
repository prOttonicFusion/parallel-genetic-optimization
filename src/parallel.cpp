/********************************************************************
 * @file      parallel.cpp
 * @author    O. Lindblom
 *
 * File containing functions related to parallel communication
 *
 * @date      2020-04-20
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "individ.hpp"
#include <algorithm>
#include <mpi.h>
#include <vector>

void getGlobalFittestRoute(Individ &globalFittest, const Individ &localFittest,
                           const std::vector<City> &cities, const int &rank, const int &Ntasks,
                           const int &tag, MPI_Comm &comm, MPI_Status &status)
{
  int Ncities = cities.size();
  std::vector<int> recvdRoute(Ncities);
  std::vector<int> localBestRoute = localFittest.route; // Locally best route
  std::vector<Individ> globalFittestCandidates(Ntasks);

  // Send the CPU's local best route to root
  if (rank != 0) MPI_Send(localBestRoute.data(), Ncities, MPI_FLOAT, 0, tag, comm);

  // Receive each sent route on root, calculate length of the route & compare it to previously
  // received routes
  if (rank == 0)
  {
    for (int i = 1; i < Ntasks; i++)
    {
      MPI_Recv(recvdRoute.data(), Ncities, MPI_FLOAT, i, tag, comm, &status);
      globalFittestCandidates[i].init(recvdRoute, cities);
    }
    globalFittestCandidates[0].init(localBestRoute, cities);
    std::sort(globalFittestCandidates.begin(), globalFittestCandidates.end());
  }

  globalFittest = globalFittestCandidates[0];
}