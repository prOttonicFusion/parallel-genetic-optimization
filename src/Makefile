
# Optimization level
OPT=-O2 

tsp_ga: main.o
	g++ main.o logic.o ${OPT} -o tsp_ga

logic.o:
	g++ -c src/logic.cpp ${OPT}

main.o:
	g++ -c src/main.cpp ${OPT}
	
clean:
	rm -f *.o

clean-all:
	rm -f *.o tsp_ga