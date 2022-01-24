/********************************************************************
 * @file      genetics.cpp
 * @author    O. Lindblom
 *
 * Functions related to genetic operations
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

// Check if city is already inlcuded in route[0:Ncities]
bool cityFoundInRoute(const std::vector<City> &route, const City &city, const int &Ncities)
{
  for (int i = 0; i < Ncities; i++)
  {
    if (route[i].id == city.id) return true;
  }
  return false;
}

// Merge the genome of two individs to produce offspring
void breedIndivids(Individ &child, const Individ &parent1, const Individ &parent2)
{
  int Ncities = parent1.Ncities;
  std::vector<City> childRoute(Ncities);

  // Get starting city from random parent
  childRoute[0] = (uniformRand(rng) < 0.5) ? parent1.route[0] : parent2.route[0];

  // Loop over genome
  for (int i = 1; i < Ncities; i++)
  {
    City nextCity;
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
      int dist1 = distanceBetweenCities(childRoute[i - 1], parent1.route[i]);
      int dist2 = distanceBetweenCities(childRoute[i - 1], parent2.route[i]);
      nextCity = (dist1 < dist2) ? parent1.route[i] : parent2.route[i];
    }
    childRoute[i] = nextCity;
  }
  child.setRoute(childRoute);
}

// Switch places of two random cities (genes) on an individual's route (genome)
void mutateIndivid(Individ &individ)
{
  std::vector<City> mutatedRoute = individ.route;

  // Generate two random array indices two switch places on
  int ind1 = (int)(individ.Ncities * uniformRand(rng));
  int ind2 = (int)(individ.Ncities * uniformRand(rng));

  // Swap array elements
  City element1 = mutatedRoute[ind1];
  mutatedRoute[ind1] = mutatedRoute[ind2];
  mutatedRoute[ind2] = element1;

  individ.setRoute(mutatedRoute);
}