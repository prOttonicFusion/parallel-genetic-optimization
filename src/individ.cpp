/********************************************************************
 * Functions for the Individ class defined in individ.hpp
 *******************************************************************/

#include "individ.hpp"
#include "city.hpp"
#include <iostream>
#include <stdio.h>

////////////////////// Individ-class functions //////////////////////

Individ::Individ()
{
}

Individ::Individ(int route[], City cities[], int Ncities)
{
  this->init(route, cities, Ncities);
}

void Individ::init(int route[], City cities[], int Ncities)
{
  // Allocate & init route array
  this->route = new int[Ncities];
  this->setRoute(route, cities, Ncities);
}

void Individ::setRoute(int route[], City cities[], int Ncities)
{
  // Copy route to Individ's internal route
  for (int i = 0; i < Ncities; i++)
  {
    this->route[i] = route[i];
  }

  // Calculate the route length
  float d = 0.0, xdiff, ydiff;
  for (int i = 1; i < Ncities; i++)
  {
    d += distanceBetweenCities(cities[route[i]], cities[route[i-1]]);
  }
  d += distanceBetweenCities(cities[route[0]], cities[route[Ncities-1]]);
  this->routeLength = d;
}

std::string Individ::getRouteAsString(City cities[], int Ncities)
{
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    int index = this->route[i];
    routeStr += cities[index].name;
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}

///////////////////// Individ operator overloads ////////////////////

// >
bool operator>(const Individ &i1, const Individ &i2)
{
  return i1.routeLength > i2.routeLength;
}

// <
bool operator<(const Individ &i1, const Individ &i2)
{
  return i1.routeLength < i2.routeLength;
}

// ==
bool operator==(const Individ &i1, const Individ &i2)
{
  return (i1.routeLength == i2.routeLength && i1.route == i2.route);
}

// !=
bool operator!=(const Individ &i1, const Individ &i2)
{
  return !(i1 == i2);
}