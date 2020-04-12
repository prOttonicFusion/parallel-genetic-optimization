/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#ifndef GENETICS_H
#define GENETICS_H
#include "city.hpp"
#include "individ.hpp"
#include <iostream>
#include <random>

/**
 * Merge the genome of two individs to produce offspring
 * This operation replaces the 2 least fittest individs with the new offspring
 **/
Individ breedIndivids(Individ parent1, Individ parent2, City cities[], int popSize, int Ncities);

/**
 * Switch places of two random cities individs route
 **/
void mutateIndivid(Individ individ, int Ncities, std::mt19937 rng);

/**
 * Get the squared distance between two cities
 **/
float distanceBetweenCities(City city1, City city2);

#endif