all:
	g++ -std=c++17 main.cpp -O3 -o main

clean:
	$(RM) main
