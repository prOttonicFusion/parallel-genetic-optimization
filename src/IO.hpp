/********************************************************************
 * Header file for IO.cpp
 *******************************************************************/
#include "individ.hpp"
#include <iostream>

bool parseXYZFile(std::string inpuFile, int &Ncities, std::string *&cityNames, float *&xpos, float *&ypos);

bool writeToOutputFile(std::string outputString, bool overWrite = false);
bool writeToOutputFile(int iterCount, int bestRoute[], std::string bestRouteStr, float bestRouteLen, int Ncities, bool overWrite = false);

void writeToScreen(int iterCount, std::string bestRouteStr, float bestRouteLen);