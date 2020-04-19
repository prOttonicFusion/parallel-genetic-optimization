/********************************************************************
 * @file      random.hpp
 * @author    O. Lindblom
 *
 * Header file containing random number engine & generators
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
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