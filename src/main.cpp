/********************************************************************
 * Main program
 *******************************************************************/
#include "IO.hpp"
#include "genetics.hpp"
#include <iostream>

using namespace std;

int main()
{
  int Ncities;            // Number of cities to use in calculationst
  float *xpos;            // The x-coordinates of each city
  float *ypos;            // The y-coordinates of each city
  std::string *cityNames; // Names of each city

  // Read city coordinates from input file
  if (!parseXYZFile(Ncities, cityNames, xpos, ypos))
  {
    cout << "Error: Could not read coordinate file" << endl;
    return -1;
  }

  int route[] = {0,1,2,3,4};
  Individ individ(route, xpos, ypos, Ncities);

  cout << individ.getRouteAsString(cityNames, Ncities) << endl;

  // TODO: Generate initial population;

  // TODO: Implement fitness function

  // TODO: Implement selection & crossover

  // TODO: Implement mutation

  if (!writeToOutputFile("test output", true))
  {
    cout << "Error: Problem with writing to ouput file" << endl;
    return -1;
  }

  return 0;
}