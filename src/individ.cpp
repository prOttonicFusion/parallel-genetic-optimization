/********************************************************************
 * Functions for the Individ class defined in individ.hpp
 *******************************************************************/

#include "individ.hpp"

////////////////////// Individ-class functions //////////////////////

/**
 * Individ constructor (no params)
 **/
Individ::Individ()
{
}

/**
 * Iinitialize new Individ
 **/
void Individ::init(int route[], float xpos[], float ypos[], int Ncities)
{
  // Allocate & init route array
  this->route = new int[Ncities];
  this->setRoute(route, xpos, ypos, Ncities);
}

/**
 * Set the individs solution (route) & calculate the distance
 **/
void Individ::setRoute(int route[], float xpos[], float ypos[], int Ncities)
{
  // Copy route to Individ's internal route
  for (int i = 0; i < Ncities; i++)
  {
    this->route[i] = route[i];
  }

  // Calculate the distance
  float d = 0.0, xdiff, ydiff;
  for (int i = 1; i < Ncities; i++)
  {
    xdiff = (xpos[route[i]] - xpos[route[i - 1]]);
    ydiff = (ypos[route[i]] - ypos[route[i - 1]]);
    d += xdiff * xdiff + ydiff * ydiff;
  }
  this->distance = d;
}

/**
 * Individ destructor
 **/
Individ::~Individ()
{
  delete[] route; // Clear memory allocated for the array
  route = NULL;   // Clear pointer
}

/**
 * Return the route as a string
 **/
std::string Individ::getRouteAsString(std::string cityNames[], int Ncities)
{
  std::string routeStr;
  //std::cout << this->route[0] << this->route[1] << this->route[2] << this->route[3] << this->route[4] << std::endl;
  for (int i = 0; i < Ncities; i++)
  {
    int index = this->route[i];
    routeStr += cityNames[index];
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}