CPP = g++-12

all : startup_code.cpp 
	@$(CPP) main.cpp -o main
	@./main alarm.bif records.dat
