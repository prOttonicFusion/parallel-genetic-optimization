/********************************************************************
 * Functions related to genetic operations
 *******************************************************************/

#include "genetics.hpp"
#include <algorithm>
#include <iostream>
#include <random>

float distanceBetweenCities(int index1, int index2, float xpos[], float ypos[])
{
  int xdiff = (xpos[index1] - xpos[index2]);
  int ydiff = (ypos[index1] - ypos[index2]);
  return xdiff * xdiff + ydiff * ydiff;
}

Individ breedIndivids(Individ parent1, Individ parent2, float xpos[], float ypos[], int popSize, int Ncities)
{
  /** 
     * Generate 2 children genoms heuristically:
     * - Take first city of one parent
     * - The Nth city in the child is chosen from the parents' Nth genes by checking 
     *   which of the two is closer to the child's N-1th city
     * - If the closer city is already found in the child, choose the other
     **/

  int childRoute[Ncities];

  childRoute[0] = parent1.route[0];

  for (int i = 1; i < Ncities; i++)
  {
    int nextCity;
    bool parent1CityInChild = std::find(childRoute, childRoute + i, parent1.route[i]) != childRoute + i;
    bool parent2CityInChild = std::find(childRoute, childRoute + i, parent2.route[i]) != childRoute + i;
    if (parent1CityInChild && parent2CityInChild)
    {
      // Both parent1.route[i] and parent2.route[i] already found in childRoute
      nextCity = 0;
      while (std::find(childRoute, childRoute + i, nextCity) != childRoute + i)
      {
        nextCity++;
      }
    }
    else if (parent1CityInChild)
    {
      // Found parent1.route[i] in childRoute
      nextCity = parent2.route[i];
    }
    else if (parent2CityInChild)
    {
      // Found parent2.route[i] in childRoute
      nextCity = parent1.route[i];
    }
    else
    {
      // Neither was found --> choose closest
      int dist1 = distanceBetweenCities(childRoute[i - 1], parent1.route[i], xpos, ypos);
      int dist2 = distanceBetweenCities(childRoute[i - 1], parent2.route[i], xpos, ypos);
      if (dist1 < dist2)
      {
        nextCity = parent1.route[i];
      }
      else
      {
        nextCity = parent2.route[i];
      }
    }
    childRoute[i] = nextCity;
  }

  return Individ(childRoute, xpos, ypos, Ncities);
}

void mutateIndivid(int indexToMutate, Individ population[], int Ncities, std::mt19937 rng)
{
  // Generate two random array indices two switch places on
  std::uniform_real_distribution<float> uniformRand(0.0, 1.0);
  int ind1 = (int)(Ncities * uniformRand(rng));
  int ind2 = (int)(Ncities * uniformRand(rng));

  // Swap array elements
  int element1 = population[indexToMutate].route[ind1];
  population[indexToMutate].route[ind1] = population[indexToMutate].route[ind2];
  population[indexToMutate].route[ind2] = element1;
}