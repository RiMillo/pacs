export LD_LIBRARY_PATH+=:../../lib
g++ -std=c++11 -c main_rk45.cpp
g++ main_rk45.o -o main_rk45
./main_rk45
