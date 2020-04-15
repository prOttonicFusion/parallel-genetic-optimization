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
#include <vector>

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

/**
 * Get the squared distance between two cities 
 * 
 * @param city1 
 * @param city2 
 * @return float Distance between city1 & city2
 */
float distanceBetweenCities(City city1, City city2);

/**
 * Convert route to string of city names 
 * 
 * @param route [in] Route as an array of indices pointing to cities
 * @param cities [in] The cities as City struct array
 * @param Ncities [in] Number of cities in route
 * @return std::string 
 */
std::string getRouteAsString(std::vector<int> route, std::vector<City> cities, int Ncities);

#endif