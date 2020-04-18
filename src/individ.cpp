/********************************************************************
 * @file      individ.cpp
 * @author    O. Lindblom
 *
 * Functions for the Individ class defined in individ.hpp
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "individ.hpp"
#include "city.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

Individ::Individ() {}

Individ::Individ(std::vector<int> newRoute, std::vector<City> cities)
{
  this->init(newRoute, cities);
}

void Individ::init(std::vector<int> newRoute, std::vector<City> cities)
{
  int NumCities = cities.size();
  this->route.resize(NumCities);
  this->Ncities = NumCities;
  this->setRoute(newRoute, cities);
}

void Individ::setRoute(std::vector<int> newRoute, std::vector<City> cities)
{
  // Copy route to Individ's internal route
  for (int i = 0; i < this->Ncities; i++)
    this->route[i] = newRoute[i];
  // this->route = newRoute;

  // Calculate the lenght of the route
  // including the distance from last city back to first
  float l = 0.0;
  for (int i = 1; i < this->Ncities; i++)
  {
    l += distanceBetweenCities(cities[route[i]], cities[route[i - 1]]);
  }

  l += distanceBetweenCities(cities[route[this->Ncities - 1]], cities[route[0]]);
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
  return (i1.routeLength == i2.routeLength && i1.route == i2.route);
}

// !=
bool operator!=(const Individ &i1, const Individ &i2) { return !(i1 == i2); }