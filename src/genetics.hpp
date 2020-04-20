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
 * @param popSize  [in] Size of population
 * @param tournamentSize  [in] Number of individuals to select randomly at random the tournament
 * @return int The index of the parent in population[]
 */
int selectParent(Individ population[], int popSize, int tournamentSize);

/**
 * Merge the genome of two individs to produce offspring
 *
 * @param child [in/out] Child individ (should be initialized prior to breeding)
 * @param parent1 [in] Mother individ
 * @param parent2 [in] Father individ
 * @param cities [in] Array of City structs
 * @param popSize [in] Size of population
 * @return Individ The offspring
 */
void breedIndivids(Individ &child, Individ parent1, Individ parent2, std::vector<City> cities,
                   int popSize);

/**
 * Switch places of two random cities on an individ's route
 *
 * @param individ [in/out] The target of the mutation
 */
void mutateIndivid(Individ individ);

#endif