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

float distanceBetweenCities(City city1, City city2)
{
  int xdiff = (city1.xpos - city2.xpos);
  int ydiff = (city1.ypos - city2.ypos);
  return xdiff * xdiff + ydiff * ydiff;
}

float getLenghtOfRoute(std::vector<int> route, std::vector<City> cities)
{
  int Ncities = cities.size();
  float l = 0.0;
  for (int i = 1; i < Ncities; i++)
  {
    l += distanceBetweenCities(cities[route[i]], cities[route[i - 1]]);
  }
  l += distanceBetweenCities(cities[route[Ncities - 1]], cities[route[0]]);
  return l;
}

std::string getRouteAsString(std::vector<int> route, std::vector<City> cities, int Ncities)
{
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    int index = route[i];
    routeStr += cities[index].name;
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}
