FLAGS=-O3  -Wall  -lm 
CXX=gcc

RM=rm -f


AI=flooditAi
MAIN= main


 
$(MAIN):  $(AI).o $(MAIN).o
	$(CXX) $(FLAGS) $(MAIN).o $(AI).o  -o $(MAIN)

$(MAIN).o: $(MAIN).c $(AI).c
	$(CXX) $(FLAGS) -c $(MAIN).c  

$(AI).o: $(AI).c
	$(CXX) $(FLAGS) -c $(AI).c 

clean:
		echo "Limpando..."
		rm -rf *.bak
		rm -rf *.o

