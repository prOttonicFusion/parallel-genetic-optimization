# Parallel Genetic Optimization

An example solution of the classic Traveling Salesman Problem (TSP) using a parallel genetic optimization algorithm in C++.

## Compilation

The program can be compiled using the `Makefile` by simply running

```
make
```

from within the `src/` directory.

## Running

The compiled program is exectuted on `<N>` processors by running

```
mpiexec -n <N> ../src/tsp_ga <coordinate-file> <Niterations> [toScreenInt] [toFileInt]
```

where `<coordinate-file>` is the xyz-file containing the city coordinates and `<Niterations>` should be the total number of generations to use in the calculation. The two last and optional parameters specify the frequency of writing data to the screen and output file, respectively. The default value of 1 means data is printed every iteration, while 0 would mean no data output.

The results can be visualized using the attached Python script, executed as

```
python3 plotRoute.py <coordinate-file>
```

## TODO

- Change population from array to STL vector?
- Move all things related to the random generator to a separate header and load that instead of passing rng as function arg
