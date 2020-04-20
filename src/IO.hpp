/********************************************************************
 * @file      IO.hpp
 * @author    O. Lindblom
 *
 * Header file for IO.cpp
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#ifndef IO_H
#define IO_H
#include "city.hpp"
#include "genetics.hpp"
#include <iostream>
#include <vector>

/**
 * Parse a xyz-file into an array of City structs
 *
 * @param [in] inputFile Path to a input file in xyz-format
 * @param [out] Ncities Number of cities in the input file
 * @param [out] cities Cities as structs in an array
 * @return true If parse succeeded
 * @return flase If an error occured
 */
bool parseXYZFile(std::string &inputFile, int &Ncities, std::vector<City> &cities);

/**
 * Write string to the output file
 *
 * @param [in] outputString Content to write to the file
 * @param [in] overWrite Should the file be overwritten?
 * @return true If write succeeded
 * @return flase If an error occured
 */
bool writeToOutputFile(const std::string &outputString, bool overWrite = false);

/**
 * Write formatted data to output file
 *
 * @param generation [in] Current generation number
 * @param bestRoute [in] Best route as int array
 * @param bestRouteStr [in] Best route as a string of city names
 * @param bestRouteLen [in] Length of the best route
 * @param overWrite [in] SHould we overwrite the file?
 * @return true If write succeeded
 * @return flase If an error occured
 */
bool writeToOutputFile(int generation, const std::vector<int> &bestRoute, const std::string &bestRouteStr,
                       const float &bestRouteLen, bool overWrite = false);

/**
 * Write formatted data to screen
 *
 * @param generation [in] Current generation number
 * @param bestRouteLen [in] Length of the best route
 */
void writeToScreen(const int &generation, const float &bestRouteLen);

#endif