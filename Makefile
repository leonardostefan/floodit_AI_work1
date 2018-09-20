FLAGS=  -Wall  -lm  -g -O3
CXX=gcc

RM=rm -f


AI=flooditAi
MAIN= main


 
$(MAIN):  $(AI).o $(MAIN).o  Makefile
	$(CXX) $(MAIN).o $(AI).o  -o $(MAIN) $(FLAGS)

$(MAIN).o: $(MAIN).c $(AI).c Makefile $(AI).h
	$(CXX)  -c $(MAIN).c   $(FLAGS)

$(AI).o: $(AI).c Makefile $(AI).h
	$(CXX) -c $(AI).c  $(FLAGS)  

clean:
		echo "Limpando..."
		rm -rf *.bak
		rm -rf *.o

