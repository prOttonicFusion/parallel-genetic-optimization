/********************************************************************
 * @file      population.cpp
 * @author    O. Lindblom (prOttonicFusion)
 *
 * Functions related to the Population class
 *
 *******************************************************************/

#include "population.hpp"
#include "city.hpp"
#include "individ.hpp"
#include "random.hpp"
#include <algorithm>
#include <vector>

// New Population object
Population::Population(const int &popSize)
{
  this->individuals.resize(popSize);
  this->populationSize = popSize;
}

// Initialize new population with random individuals
void Population::init(std::vector<City> cities, const int &Ncities)
{
  std::vector<City> route = cities;

  for (int i = 0; i < this->populationSize; i++)
  {
    std::shuffle(route.begin(), route.end(), rng); // New random route by shuffling cities
    this->individuals[i].init(route);
  }
}

// Sort the population in ascending order based on route length
void Population::sort() { std::sort(this->individuals.begin(), this->individuals.end()); }

// Select tournamentSize random individuals from population and save the index of the fittest
Individ Population::selectRandomIndivid(const int &tournamentSize)
{
  int bestIndex = uniformRand(rng) * this->populationSize;
  for (int i = 0; i < tournamentSize - 1; i++)
  {
    int index = uniformRand(rng) * populationSize;
    if (this->individuals[index].routeLength < this->individuals[bestIndex].routeLength)
      bestIndex = index;
  }
  return this->individuals[bestIndex];
}