/********************************************************************
 * Header file for IO.cpp
 *******************************************************************/ 
#include <iostream>

bool parseXYZFile(int &Ncities, std::string*& cityNames, float*& xpos, float*& ypos);

bool writeToOutputFile(std::string outputString, bool overWrite);

void writeToScreen();