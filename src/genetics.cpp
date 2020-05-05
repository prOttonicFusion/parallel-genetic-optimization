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

int selectRandomIndivid(Individ population[], const int &populationSize, const float &routeLengthSum)
{
  // Select inidividual using Fitness Proportionate Selection 
  // Calculate probability as p_i = (1.0/routeLength_i)/(1.0/routeLengthSum)
  double randomValue = uniformRand(rng);
  double offset = 0.0;
  int selectedIndex = 0;

  for (int i = 0; i < populationSize; i++)
  {
    offset += 1.0/population[i].routeLength*routeLengthSum;
    if (randomValue < offset)
    {
      selectedIndex = i;
      break;
    }
  }

  return selectedIndex;
}

bool cityFoundInRoute(const std::vector<int> &route, const int &cityIndex, const int &Ncities)
{
  for (int i = 0; i < Ncities; i++)
  {
    if (route[i] == cityIndex) return true;
  }
  return false;
}

void breedIndivids(Individ &child, const Individ &parent1, const Individ &parent2,
               const std::vector<City> &cities, const int &populationSize){

     std::vector<int> childRoute(parent1.Ncities);
     for (int i = 0; i < parent1.Ncities; i++)
     {
       childRoute[i] = -1;
     }

    // Get start and end sub tour positions for parent1's tour
    int startPos = (int) (uniformRand(rng) * parent1.Ncities);
    int endPos = (int) uniformRand(rng) * parent1.Ncities;

    // Loop and add the sub tour from parent1 to our child
    for (int i = 0; i < parent1.Ncities; i++) {
        // If our start position is less than the end position
        if (startPos < endPos && i > startPos && i < endPos) {
            childRoute[i] = parent1.route[i];
        } // If our start position is larger
        else if (startPos > endPos) {
            if (!(i < startPos && i > endPos)) {
                childRoute[i] = parent1.route[i];
            }
        }
    }

    // Loop through parent2's city tour
    for (int i = 0; i < parent1.Ncities; i++) {
        // If child doesn't have the city add it
        if (!cityFoundInRoute(childRoute, parent2.route[i], i)) {
            // Loop to find a spare position in the child's tour
            for (int ii = 0; ii < parent1.Ncities; ii++) {
                // Spare position found, add city
                if (childRoute[ii] == -1) {
                    childRoute[ii] = parent2.route[ii];
                    break;
                }
            }
        }
    }
    child.setRoute(childRoute, cities);
}

void mutateIndivid(Individ &individ)
{
  // Generate two random array indices two switch places on
  int ind1 = (int)(individ.Ncities * uniformRand(rng));
  int ind2 = (int)(individ.Ncities * uniformRand(rng));

  // Swap array elements
  int element1 = individ.route[ind1];
  individ.route[ind1] = individ.route[ind2];
  individ.route[ind2] = element1;
}
