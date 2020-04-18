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
 * @param rng [in] Random engine
 */
void mutateIndivid(Individ individ, std::mt19937 rng);

#endif