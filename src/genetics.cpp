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

int selectParent(Individ population[], int popSize, int tournamentSize)
{
  // Select tournamentSize random individuals from population and save the index of the fittest
  int bestIndex = uniformRand(rng) * popSize;
  for (int i = 0; i < tournamentSize - 1; i++)
  {
    int index = uniformRand(rng) * popSize;
    if (population[index].routeLength < population[bestIndex].routeLength) bestIndex = index;
  }

  return bestIndex;
}

bool cityAlreadyInRoute(std::vector<int> route, int cityIndex, int Ncities)
{
  for (int i = 0; i < Ncities; i++)
  {
    if (route[i] == cityIndex) return true;
  }
  return false;
}

void breedIndivids(Individ &child, Individ parent1, Individ parent2, std::vector<City> cities,
                   int popSize)
{
  /**
   * Generate 2 children genoms heuristically:
   * - Take first city of one parent
   * - The Nth city in the child is chosen from the parents' Nth genes by checking
   *   which of the two is closer to the child's N-1th city
   * - If the closer city is already found in the child, choose the other
   **/
  int Ncities = parent1.Ncities;
  std::vector<int> childRoute(Ncities);

  // Select starting city randomly from either parent
  childRoute[0] = (uniformRand(rng) > 0.5) ? parent1.route[0] : parent2.route[0];

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

void mutateIndivid(Individ individ, std::mt19937 rng)
{
  // Generate two random array indices two switch places on
  std::uniform_real_distribution<float> uniformRand(0.0, 1.0);
  int ind1 = (int)(individ.Ncities * uniformRand(rng));
  int ind2 = (int)(individ.Ncities * uniformRand(rng));

  // Swap array elements
  int element1 = individ.route[ind1];
  individ.route[ind1] = individ.route[ind2];
  individ.route[ind2] = element1;
}