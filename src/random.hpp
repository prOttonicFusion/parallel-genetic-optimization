/********************************************************************
 * @file      random.hpp
 * @author    O. Lindblom (prOttonicFusion)
 *
 * Random number engine & generators
 *
 *******************************************************************/

#ifndef RANDOM_H
#define RANDOM_H
#include <random>

// Random seed generator
static std::random_device rd;

// Random engine (Mersenne Twister)
static std::mt19937 rng(rd());

// Uniform random number generator
static std::uniform_real_distribution<float> uniformRand(0.0, 1.0);

#endif