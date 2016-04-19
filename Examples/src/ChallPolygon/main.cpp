#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "grid.hpp"

using namespace std;

int main(int argc, char** argv) {
	string file("mesh.dat");
	//Reading constructor
	Grid g(file);
	//Default constructor plus read_set method
	//Grid g;
	//g.read_set(file);
	
	cout << g.sum_area()<< endl;
	return 0;
}
