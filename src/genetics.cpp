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

int selectParent(Individ population[], const int &populationSize, const int &tournamentSize)
{
  // Select tournamentSize random individuals from population and save the index of the fittest
  int bestIndex = uniformRand(rng) * populationSize;
  for (int i = 0; i < tournamentSize - 1; i++)
  {
    int index = uniformRand(rng) * populationSize;
    if (population[index].routeLength < population[bestIndex].routeLength) bestIndex = index;
  }

  return bestIndex;
}

bool cityAlreadyInRoute(const std::vector<int> &route, const int &cityIndex, const int &Ncities)
{
  for (int i = 0; i < Ncities; i++)
  {
    if (route[i] == cityIndex) return true;
  }
  return false;
}

void breedIndivids(Individ &child, const Individ &parent1, const Individ &parent2,
                   const std::vector<City> &cities, const int &populationSize)
{
  int Ncities = parent1.Ncities;
  std::vector<int> childRoute(Ncities);

  // Select starting city randomly from either parent
  childRoute[0] = parent1.route[0];

  for (int i = 1; i < Ncities; i++)
  {
    int nextCity = 0;
    bool parent1CityInChild = cityAlreadyInRoute(childRoute, parent1.route[i], i);
    bool parent2CityInChild = cityAlreadyInRoute(childRoute, parent2.route[i], i);
    if (parent1CityInChild && parent2CityInChild)
    {
      // Both parent1.route[i] and parent2.route[i] already found in childRoute
      nextCity = 0;
      while (cityAlreadyInRoute(childRoute, nextCity, i))
      {
        nextCity++;
      }
    }
    else if (parent1CityInChild)
    {
      // Found parent1.route[i] in childRoute
      nextCity = parent2.route[i];
    }
    else if (parent2CityInChild)
    {
      // Found parent2.route[i] in childRoute
      nextCity = parent1.route[i];
    }
    else
    {
      // Neither was found --> choose closest
      int dist1 = distanceBetweenCities(cities[childRoute[i - 1]], cities[parent1.route[i]]);
      int dist2 = distanceBetweenCities(cities[childRoute[i - 1]], cities[parent2.route[i]]);
      if (dist1 < dist2)
      {
        nextCity = parent1.route[i];
      }
      else
      {
        nextCity = parent2.route[i];
      }
    }
    childRoute[i] = nextCity;
  }
  child.setRoute(childRoute, cities);
}

void mutateIndivid(Individ &individ)
{
  // Generate two random array indices two switch places on
  int ind1 = (int)(individ.Ncities * uniformRand(rng));
  int ind2 = (int)(individ.Ncities * uniformRand(rng));

  // Swap array elements
  int element1 = individ.route[ind1];
  individ.route[ind1] = individ.route[ind2];
  individ.route[ind2] = element1;
}