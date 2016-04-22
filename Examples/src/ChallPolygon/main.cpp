#include <iostream>
#include <string>
#include "grid.hpp"

using namespace std;

int main(int argc, char** argv) {
	string file("mesh.dat");
	//Reading constructor
	Grid g(file);
	//Default constructor plus read_set method
	//Grid g;
	//g.read_set(file);
	
	//For debugging purpose
	//g.print();
	//g.print_area();
	
	cout << g.sum_area()<< endl;

	//For debugging purpose
	//g.show();
	//cout << endl;

	g.print_edges("all.txt","bord.txt","internal.txt");

	return 0;
}
