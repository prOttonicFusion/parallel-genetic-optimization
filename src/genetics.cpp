/********************************************************************
 * @file      genetics.cpp
 * @author    O. Lindblom
 *
 * Functions related to genetic operations
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "genetics.hpp"
#include "city.hpp"
#include "individ.hpp"
#include "random.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

// Select tournamentSize random individuals from population and save the index of the fittest
int selectRandomIndivid(Individ population[], const int &populationSize, const int &tournamentSize)
{
  int bestIndex = uniformRand(rng) * populationSize;
  for (int i = 0; i < tournamentSize - 1; i++)
  {
    int index = uniformRand(rng) * populationSize;
    if (population[index].routeLength < population[bestIndex].routeLength) bestIndex = index;
  }
  return bestIndex;
}

// Check if cityIndex is already inlcuded in route[0:Ncities]
bool cityFoundInRoute(const std::vector<int> &route, const int &cityIndex, const int &Ncities)
{
  for (int i = 0; i < Ncities; i++)
  {
    if (route[i] == cityIndex) return true;
  }
  return false;
}

// Merge the genome of two individs to produce offspring
void breedIndivids(Individ &child, const Individ &parent1, const Individ &parent2,
                   const std::vector<City> &cities)
{
  int Ncities = parent1.Ncities;
  std::vector<int> childRoute(Ncities);

  // Get starting city from random parent
  childRoute[0] = (uniformRand(rng) < 0.5) ? parent1.route[0] : parent2.route[0];

  // Loop over genome
  for (int i = 1; i < Ncities; i++)
  {
    int nextCity = 0;
    bool parent1CityFoundInChild = cityFoundInRoute(childRoute, parent1.route[i], i);
    bool parent2CityFoundInChild = cityFoundInRoute(childRoute, parent2.route[i], i);
    if (parent1CityFoundInChild && parent2CityFoundInChild)
    {
      // Both parent1.route[i] and parent2.route[i] already found in childRoute
      // Select next city of one of the parents
      for (int j = 1; j < Ncities; j++)
      {
        int index = (i + j < Ncities) ? i + j : j - (Ncities - i); // Periodic boundaries
        bool parent1NextCityFoundInChild = cityFoundInRoute(childRoute, parent1.route[index], i);
        bool parent2NextCityFoundInChild = cityFoundInRoute(childRoute, parent2.route[index], i);

        if (!parent1NextCityFoundInChild && !parent2NextCityFoundInChild)
          nextCity = (uniformRand(rng) < 0.5) ? parent1.route[index] : parent2.route[index];
        else if (parent1NextCityFoundInChild && !parent2NextCityFoundInChild)
          nextCity = parent2.route[index];
        else if (parent2NextCityFoundInChild && !parent1NextCityFoundInChild)
          nextCity = parent1.route[index];
        else
          continue;

        if (!cityFoundInRoute(childRoute, nextCity, i)) break;
      }
    }
    else if (parent1CityFoundInChild)
    {
      // Found parent1.route[i] in childRoute
      nextCity = parent2.route[i];
    }
    else if (parent2CityFoundInChild)
    {
      // Found parent2.route[i] in childRoute
      nextCity = parent1.route[i];
    }
    else
    {
      // Neither was found --> choose closest
      int dist1 = distanceBetweenCities(cities[childRoute[i - 1]], cities[parent1.route[i]]);
      int dist2 = distanceBetweenCities(cities[childRoute[i - 1]], cities[parent2.route[i]]);
      nextCity = (dist1 < dist2) ? parent1.route[i] : parent2.route[i];
    }
    childRoute[i] = nextCity;
  }
  child.setRoute(childRoute, cities);
}

// Switch places of two random cities (genes) on an individual's route (genome)
void mutateIndivid(Individ &individ, const std::vector<City> &cities)
{
  std::vector<int> mutatedRoute = individ.route;

  // Generate two random array indices two switch places on
  int ind1 = (int)(individ.Ncities * uniformRand(rng));
  int ind2 = (int)(individ.Ncities * uniformRand(rng));

  // Swap array elements
  int element1 = mutatedRoute[ind1];
  mutatedRoute[ind1] = mutatedRoute[ind2];
  mutatedRoute[ind2] = element1;

  individ.setRoute(mutatedRoute, cities);
}