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

float distanceBetweenCities(City city1, City city2)
{
  int xdiff = (city1.xpos - city2.xpos);
  int ydiff = (city1.ypos - city2.ypos);
  return xdiff * xdiff + ydiff * ydiff;
}
