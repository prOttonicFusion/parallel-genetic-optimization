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

#include "city.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include "population.hpp"
#include <algorithm>
#include <mpi.h>
#include <vector>

// Compares the shortest routes on each CPU and determine the globally shortest one
void getGlobalFittestRoute(Individ &globalFittest, const Individ &localFittest, const int &rank,
                           const int &Ncities, const int &Ntasks, const int &tag, MPI_Comm &comm,
                           MPI_Status &status)
{
  std::vector<City> recvdRoute(Ncities);
  std::vector<City> localBestRoute = localFittest.route; // Locally best route
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
      globalFittestCandidates[i].init(recvdRoute);
    }
    globalFittestCandidates[0].init(localBestRoute);
    std::sort(globalFittestCandidates.begin(), globalFittestCandidates.end());
    globalFittest = globalFittestCandidates[0];
  }
}

// Compares the shortest routes on each CPU and determine the globally shortest one
void getGlobalFittestRouteLenght(float &globalShortestRouteLength, const Individ &localFittest,
                                 const int &rank, const int &Ntasks, MPI_Comm &comm)
{
  float bestRouteLen = localFittest.routeLength;
  float localShortestRoutes[Ntasks]; // The lengths of each process' fittest individual

  // Gather the lenghts of the best routes from all CPUs to CPU 0
  MPI_Gather(&bestRouteLen, 1, MPI_FLOAT, localShortestRoutes, 1, MPI_FLOAT, 0, comm);

  if (rank == 0)
  {
    // Find globally shortest route & print it to screen
    std::sort(localShortestRoutes, localShortestRoutes + Ntasks);
    globalShortestRouteLength = localShortestRoutes[0];
  }
}

// Copy over migrationSize random individuals from each CPU to its right-side neighbor in a circular
void performMigration(const int &migrationSize, const int &tournamentSize, Population population,
                      const int &Ncities, const int &rank, const int &Ntasks, const int &tag,
                      MPI_Comm &GRID_COMM, MPI_Status &status)
{
  std::vector<City> recvdRoute(Ncities);
  float recvdRouteLength;
  int destRank, sourceRank;

  // Get receiver rank for closest neighbor communication
  MPI_Cart_shift(GRID_COMM, 0, 1, &sourceRank, &destRank);

  for (int i = 0; i < migrationSize; i++)
  {
    // Select random individual to send
    Individ randomIndivid = population.selectRandomIndivid(tournamentSize);

    // Send & receive individuals
    MPI_Sendrecv(randomIndivid.route.data(), Ncities, MPI_INT, destRank, tag, recvdRoute.data(),
                 Ncities, MPI_INT, sourceRank, tag, GRID_COMM, &status);
    MPI_Sendrecv(&randomIndivid.routeLength, 1, MPI_FLOAT, destRank, tag, &recvdRouteLength, 1,
                 MPI_FLOAT, sourceRank, tag, GRID_COMM, &status);
    // Add route received from neighbor to own population by replacing own least fit indviduals
    // Skip if own least fit are more fit than the new candidates
    if (recvdRouteLength < population.individuals[population.populationSize - (i + 1)].routeLength)
      population.individuals[population.populationSize - (i + 1)].setRoute(recvdRoute);
  }
}