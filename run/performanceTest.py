#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################################################
# @file      performanceTest.py
# @author    O. Lindblom
#
# Test the performance of the C++ code using varying numbers of CPUs
#
# @date      2020-04-13
# @copyright Copyright (c) 2020
#####################################################################

import timeit
import subprocess
import numpy as np
import matplotlib.pyplot as plt

Nrepeat = 20           # Run program Nrepeat times for each setup & take average
Nproc = [1, 2, 3, 4, 5, 6, 7, 8]  # The processor setups to use
# Command line rguments for the C++ program
inputArgs = ['randomCities.xyz', '1000', '100', '100']


def tail(f, n):
    """Get last n lines of file"""
    proc = subprocess.Popen(
        ['tail', '-n', str(n), f], stdout=subprocess.PIPE)
    lines = proc.stdout.readlines()
    return lines[:]


def getShortestRouteLength():
    """Extact the shortest route length from output.dat"""
    lastOutputFrame = tail('output.dat', 5)
    return float(lastOutputFrame[1].split()[1])


# Run the program multiple times for each setup and calculate an average final route length and wall-time
avgFinalLength_MPI = np.zeros(len(Nproc))
avgTimes_MPI = np.zeros(len(Nproc))
for i, N in enumerate(Nproc):
    print('Running on {} CPUs'.format(N))
    routeLengthSum = 0.0
    timeSum = 0.0
    for j in range(Nrepeat):
        runCmd = "subprocess.run(['mpiexec', '-n', '{}', '../src/tsp_ga', '{}', '{}', '{}', '{}'])".format(
            N, *inputArgs)
        timeSum += timeit.timeit(stmt=runCmd,
                                 setup="import subprocess", number=1)
        routeLengthSum += getShortestRouteLength()
    avgFinalLength_MPI[i] = routeLengthSum/Nrepeat
    avgTimes_MPI[i] = timeSum/Nrepeat

print(avgFinalLength_MPI)

############################ Draw figure ############################
plt.figure(num=0, figsize=[7, 4])
plt.plot(Nproc, avgTimes_MPI, 'ro--')
plt.xlabel("Number of processes")
plt.ylabel("Mean Wall-time (s)")
# plt.legend()
plt.savefig("performance_walltime.png")
plt.show()

plt.figure(num=1, figsize=[7, 4])
plt.plot(Nproc, avgFinalLength_MPI, 'o--')
plt.xlabel("Number of processes")
plt.ylabel("Mean Final Squared Route Length")
plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
# plt.legend()
plt.savefig("performance_route.png")
plt.show()

plt.figure(num=2, figsize=[7, 4])
plt.plot(Nproc, [avgFinalLength_MPI[i]*avgTimes_MPI[i] for i in range(len(Nproc))], 'go--')
plt.xlabel("Number of processes")
plt.ylabel("Route Length * Wall-time")
plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
# plt.legend()
plt.savefig("performance_relative.png")
plt.show()
