CPP = g++-12

all : startup_code.cpp 
	@$(CPP) startup_code.cpp -o hello
	@./hello alarm.bif records.dat
