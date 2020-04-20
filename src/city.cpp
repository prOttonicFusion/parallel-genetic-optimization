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

float distanceBetweenCities(const City &city1, const City &city2)
{
  int xdiff = (city1.xpos - city2.xpos);
  int ydiff = (city1.ypos - city2.ypos);
  return xdiff * xdiff + ydiff * ydiff;
}

std::string getRouteAsString(const std::vector<int> &route, const std::vector<City> &cities)
{
  int Ncities = cities.size();
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    int index = route[i];
    routeStr += cities[index].name;
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}
