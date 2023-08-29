num = 1

test: src/SportsLayout.cpp
	g++ -o main.exe src/SportsLayout.cpp
	./main.exe test_cases/input$(num).txt output/output$(num).txt