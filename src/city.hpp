/********************************************************************
 * @file      city.hpp
 * @author    O. Lindblom
 * 
 * Header file for declaration of City struct
 * 
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 * 
 *******************************************************************/

#ifndef CITY_H
#define CITY_H
#include <iostream>

/**
 * A data structure for storing all properties of a single city (gene)
 **/

/**
 * Data structure for storing a city's name and position as
 * (string) City.name, (float) City.xpos, (float) City.ypos
 * 
 */
struct City
{
  std::string name;
  float xpos;
  float ypos;
};

#endif