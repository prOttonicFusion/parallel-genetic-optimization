/********************************************************************
 * @file      population.hpp
 * @author    O. Lindblom
 *
 * Header file for population.cpp. Contains the Population class
 *
 * @date      2020-05-8
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#ifndef POPULATION_H
#define POPULATION_H
#include "individ.hpp"
#include <vector>

class Population
{
  public:
  std::vector<Individ> individuals;
  int populationSize;

  /**
   * Construct a new Population object
   *
   * @param populationSize  [in] Number of individuals in the population
   */
  Population(const int &populationSize);

  /**
   * Initialize Population with random individuals
   *
   * @param cities  [in] A vector of City objects
   * @param Ncities  [in] Number of cities that should be along one route
   */
  void init(std::vector<City> cities, const int &Ncities);

  /**
   * Sort population in ascending order based on route length
   *
   */
  void sort();

  /**
   * Select randomly an individual from the population using Tournament Selection; i.e. by selecting
   * the fittest out of tournamentSize random individuals
   *
   * @param tournamentSize  [in] Number of individuals to participate in the tournament
   * @return Individ The randomly selected Individ object
   */
  Individ selectRandomIndivid(const int &tournamentSize);

};

#endif