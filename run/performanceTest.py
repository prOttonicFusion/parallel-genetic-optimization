#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################################################
# Test the performance of the C++ code using varying numbers of CPUs
#
######################################################################

import timeit
import subprocess
import numpy as np
import matplotlib.pyplot as plt

Nrepeat = 5          # Run program this many times for each setup & take average
Nproc = [1, 2, 3, 4]  # The processor setups to use
# Command line rguments for the C++ program
inputArgs = ['randomCities.xyz', 1000, 1, 1]

# Run the program multiple times for each setup and calculate an average wall-time
avgTimes_MPI = np.zeros(len(Nproc))
for i, N in enumerate(Nproc):
    runCmd = "subprocess.run(['mpiexec', '-n', '{}', '../src/tsp_ga', '{}', '{}', '{}', '{}'])".format(N, *inputArgs)
    print(runCmd)
    avgTimes_MPI[i] = timeit.timeit(
        stmt=runCmd, setup="import subprocess", number=Nrepeat)

############################ Draw figure ############################
plt.figure(num=1, figsize=[8, 5])
plt.plot(Nproc, avgTimes_MPI, 'ro--')
plt.xlabel("Number of processes")
plt.ylabel("Wall-time (s)")
# plt.legend()
plt.savefig("performance.png")
plt.show()
