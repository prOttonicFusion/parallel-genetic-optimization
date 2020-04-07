/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include <iostream>

using namespace std;

int main()
{
  int Ncities;            // Number of cities to use in calculationst
  float *xpos;            // The x-coordinates of each city
  float *ypos;            // The y-coordinates of each city
  std::string *cityNames; // Names of each city

  if (!parseXYZFile(Ncities, cityNames, xpos, ypos))
  {
    cout << "Error: Could not read coordinate file" << endl;
    return -1;
  }

  cout << Ncities << " cities, first of which is " << cityNames[0] << " at x=" << xpos[0] << ", y=" << ypos[0] << endl;

  return 0;
}