
# Optimization level
OPT=-O2 

tsp_ga: IO.o individ.o genetics.o main.o
	g++ -g main.o IO.o individ.o genetics.o ${OPT} -o tsp_ga

IO.o:
	g++ -c IO.cpp ${OPT}

individ.o:
	g++ -c individ.cpp ${OPT}

genetics.o:
	g++ -c genetics.cpp ${OPT}

main.o:
	g++ -c main.cpp ${OPT}
	
clean:
	rm -f *.o

clean-all:
	rm -f *.o tsp_ga