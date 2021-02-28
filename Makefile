simulation: main.cpp
	g++ -std=c++11 -lpthread main.cpp -o simulation 
clean:
	rm *.o simulation

