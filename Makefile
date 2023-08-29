num = 1

test: source/SportsLayout.cpp
	g++ -o main source/SportsLayout.cpp
	./main test_cases/input$(num).txt output/output$(num).txt