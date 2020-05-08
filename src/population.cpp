/********************************************************************
 * @file      population.cpp
 * @author    O. Lindblom
 *
 * Functions related to the Population class
 *
 * @date      2020-05-8
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "population.hpp"
#include "city.hpp"
#include "individ.hpp"
#include "random.hpp"
#include <algorithm>
#include <vector>

// New Population object
Population::Population(int popSize)
{
  this->population.resize(popSize);
  this->populationSize = popSize;
}

void Population::init(std::vector<City> cities, int Ncities)
{
  std::vector<int> route; // Array containing city indices in a specific order
  route.resize(Ncities);
  for (int i = 0; i < Ncities; i++)
    route[i] = i;

  for (int i = 0; i < this->populationSize; i++)
  {
    std::shuffle(route.begin(), route.end(), rng); // New random route by shuffling cities
    this->population[i].init(route, cities);
  }
}

void Population::sort() { std::sort(this->population.begin(), this->population.end()); }