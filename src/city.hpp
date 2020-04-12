/********************************************************************
 * Header file for declaration of City struct
 *******************************************************************/

#ifndef CITY_H
#define CITY_H
#include <iostream>

/**
 * A data structure for storing all properties of a single city (gene)
 **/
struct City
{
  std::string name;
  float xpos;
  float ypos;
};

#endif