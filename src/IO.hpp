/********************************************************************
 * Header file for IO.cpp
 *******************************************************************/

#ifndef IO_H
#define IO_H
#include "city.hpp"
#include "genetics.hpp"
#include <iostream>

bool parseXYZFile(std::string inpuFile, int &Ncities, City *&cities);

bool writeToOutputFile(std::string outputString, bool overWrite = false);
bool writeToOutputFile(int iterCount, int bestRoute[], std::string bestRouteStr, float bestRouteLen, int Ncities, bool overWrite = false);

void writeToScreen(int iterCount, std::string bestRouteStr, float bestRouteLen);

#endif