FLAGS=  -Wall  -lm  -g -O0
CXX=gcc

RM=rm -f



MAIN= main


 
$(MAIN):  floodit.o heuristic.o $(MAIN).o  Makefile
	$(CXX) $(MAIN).o floodit.o heuristic.o  -o $(MAIN) $(FLAGS)

$(MAIN).o: $(MAIN).c floodit.c heuristic.c Makefile
	$(CXX)  -c $(MAIN).c   $(FLAGS)

floodit.o: floodit.c Makefile floodit.h
	$(CXX) -c floodit.c  $(FLAGS)  

heuristic.o: heuristic.c Makefile heuristic.h floodit.h floodit.c
	$(CXX) -c heuristic.c  $(FLAGS)  
clean:
		echo "Limpando..."
		rm -rf *.bak
		rm -rf *.o

