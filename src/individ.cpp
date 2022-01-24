/********************************************************************
 * @file      individ.cpp
 * @author    O. Lindblom
 *
 * The Individ class
 *
 *******************************************************************/

#include "individ.hpp"
#include "city.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

// Construct a new blank Individ object
Individ::Individ() {}

// Construct a new Individ object from the provided route
Individ::Individ(const std::vector<City> &newRoute)
{
  this->init(newRoute);
}

// Initialize Individ with route. Same as setRoute, but also resize the route vector
void Individ::init(const std::vector<City> &newRoute)
{
  this->Ncities = newRoute.size();
  this->setRoute(newRoute);
}

// Set the route of the Individ object & re-calculate the route length
void Individ::setRoute(const std::vector<City> &newRoute)
{
  // Copy route to Individ's internal route
  this->route = newRoute;

  // Calculate the lenght of the route, including the distance from last city back to first
  float l = 0.0;
  for (int i = 1; i < this->Ncities; i++)
  {
    l += distanceBetweenCities(route[i], route[i - 1]);
  }

  l += distanceBetweenCities(route[this->Ncities - 1], route[0]);
  this->routeLength = l;
}

/**
 * Operator overloadings
 */

// >
bool operator>(const Individ &i1, const Individ &i2) { return i1.routeLength > i2.routeLength; }

// <
bool operator<(const Individ &i1, const Individ &i2) { return i1.routeLength < i2.routeLength; }

// ==
bool operator==(const Individ &i1, const Individ &i2)
{
  return (i1.routeLength == i2.routeLength && i1.route.size() == i2.route.size());
}

// !=
bool operator!=(const Individ &i1, const Individ &i2) { return !(i1 == i2); }