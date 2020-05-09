/********************************************************************
 * @file      city.cpp
 * @author    O. Lindblom
 *
 * File containing functions related to the City struct
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "city.hpp"
#include <vector>

// Get the squared distance between two cities
float distanceBetweenCities(const City &city1, const City &city2)
{
  int xdiff = (city1.xpos - city2.xpos);
  int ydiff = (city1.ypos - city2.ypos);
  return xdiff * xdiff + ydiff * ydiff;
}

// Convert a route in array-of-indices form to a string of city names
std::string getRouteAsString(const std::vector<City> &route)
{
  int Ncities = route.size();
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    routeStr += std::to_string(route[i].id);
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}
