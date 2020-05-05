/********************************************************************
 * @file      genetics.hpp
 * @author    O. Lindblom
 *
 * Header file for genetics.cpp
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#ifndef GENETICS_H
#define GENETICS_H
#include "city.hpp"
#include "individ.hpp"
#include <iostream>
#include <random>
#include <vector>

/**
 * Select randomly a parent from the population using Tournament Selection
 *
 * @param population  [in] The population, i.e. an array of Individ objects
 * @param populationSize  [in] Size of population
 * @param tournamentSize  [in] Number of individuals to select randomly at random the tournament
 * @return int The index of the parent in population[]
 */
int selectRandomIndivid(Individ population[], const int &populationSize, const int &tournamentSize);

/**
 * Merge the genome of two individs to produce offspring.
 *
 * The genome crossover occurs heurustically:
 * 1) 0th city of one parent is set as the child's 0th city
 * 2) The Nth city in the child is chosen from the parents' Nth genes by checking
 *    which of the two is closer to the child's N-1th city
 * 3) If the closer city is already found in the child, take the city from the other parent
 * 4) If both are already found in child, pick next city from either parent
 *
 * @param child [in/out] Child individ (should be initialized prior to breeding)
 * @param parent1 [in] Mother individ
 * @param parent2 [in] Father individ
 * @param cities [in] Array of City structs
 * @param populationSize [in] Size of population
 */
void breedIndivids(Individ &child, const Individ &parent1, const Individ &parent2,
                   const std::vector<City> &cities, const int &populationSize);

/**
 * Switch places of two random cities (genes) on an individ's route (genome)
 *
 * @param individ [in/out] The individual to mutate
 * @param cities [in] Array of City structs
 */
void mutateIndivid(Individ &individ, const std::vector<City> &cities);

#endif