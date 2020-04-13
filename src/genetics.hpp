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

/**
 * Merge the genome of two individs to produce offspring
 * 
 * @param parent1 [in] Mother individ
 * @param parent2 [in] Father individ
 * @param cities [in] Array of City structs
 * @param popSize [in] Size of population
 * @param Ncities [in] Number of cities in one route
 * @return Individ The offspring
 */
Individ breedIndivids(Individ parent1, Individ parent2, City cities[], int popSize, int Ncities);

/**
 * Switch places of two random cities on an individ's route
 * 
 * @param individ [in/out] The target of the mutation 
 * @param Ncities [in] Array of City structs
 * @param rng [in] Random engine
 */
void mutateIndivid(Individ individ, int Ncities, std::mt19937 rng);

/**
 * Get the squared distance between two cities 
 * 
 * @param city1 
 * @param city2 
 * @return float Distance between city1 & city2
 */
float distanceBetweenCities(City city1, City city2);

#endif