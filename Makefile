all:
	g++ -std=c++20 main.cpp -O3 -o main

clean:
	$(RM) main
