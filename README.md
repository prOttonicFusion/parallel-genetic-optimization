# Parallel Genetic Optimization

A multiple-deme/stepping-stone parallel genetic solver for the classical Traveling Salesman Problem (TSP). Written in C++ using the [Open MPI](https://www.open-mpi.org/) Message Passing Interface implementation.

| ![Initial route image](/images/initial_route.png) | ![Final route image](/images/final_route.png) |
|:--:|:--:| 
| *Initial route* | *Final route (population of 100, after 50 generations)* |

## Compilation

The program can be compiled using the `Makefile` by simply running

```
make
```

from within the `src/` directory.

## Running

The compiled program is exectuted on `<N>` processors by running

```
mpiexec -n <N> ../src/tsp_ga <coordinate-file> <NGenerations> [wrtToScreen] [wrtToFile]
```

where `<coordinate-file>` is the path to a xyz-file containing the city coordinates and `<NGenerations>` should be the total number of generations to use in the calculation. The two last and optional parameters specify the frequency of writing data to the screen and output file, respectively. The default value of 1 means data is printed every generation, while 0 would mean no data output. Additional calculation parameters, such as the population size can be adjusted in the `input.dat` file. Sample input files are found in the `run/` directory.

The results of a successful calculation can be visualized using the attached Python script, executed as

```
python3 plotRoute.py <coordinate-file> <output-file>
```
