#include <iostream>
#include <string>
//#include <set>
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

	//set<Edge> se;
	//se.emplace(0,1);
	//se.emplace(1,3);
	//se.emplace(0,1);
	//for(auto ss : se)
		//cout << ss;
	return 0;
}
