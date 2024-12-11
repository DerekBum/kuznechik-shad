all:
	g++ -std=c++20 main.cpp ./src/* -I ./lib -O3 -o main

clean:
	$(RM) main
