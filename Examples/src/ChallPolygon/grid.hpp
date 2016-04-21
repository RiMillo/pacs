#ifndef HH_GRID_HH
#define HH_GRID_HH

#include "Polygon.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <string>

using namespace Geometry;

class Edge
{
private:
	unsigned int a;
	unsigned int b;
public:
	//Edge():a(0),b(0) {}
	Edge(unsigned int aa=0, unsigned int bb=0) {
		//Check if true edge (different points)
		//assert(aa!=bb);
		std::pair<unsigned int, unsigned int> ab=std::minmax({aa,bb});
		a=ab.first;
		b=ab.second;
	}
	Edge(Edge const & e) {
		a=e.a; b=e.b;
	}
	bool operator<(Edge const & f) const {
		if(this->a<f.a) return true;
		else if (this->a==f.a && this->b<f.b) return true;
		else return false;
	}
	friend std::ostream & operator<< (std::ostream & o, Edge & e) {
		o<<"("<<e.a<<","<<e.b<<")"<<std::endl;
		return o;
	}
	friend class Grid;
};//END CLASS EDGE

class Grid
{
private:
	std::vector<Point2D> vpt;
	std::vector<std::shared_ptr<AbstractPolygon>> vap;
	std::vector<::Edge> all;
	std::vector<::Edge> bord; 
public:
	//! Ctors
	Grid() = default;
	Grid(Grid const & g) = default;
	Grid(std::string const & file);

	void read_set(std::string const & file);
	double sum_area();
	void clear() {vpt.clear(); vap.clear();}

	Grid& operator = (Grid const & g)=default;

	void print() {
		std::cout << vpt.size() << " point(s)\n";
		std::cout << vap.size() << " polygon(s)\n";
	}
	void print_area() {
		for(auto it=vap.begin(); it!=vap.end();++it)
			std::cout << (*it)->area()<<std::endl;
	}
	

	void show() {
		std::cout << "All edges:"<<std::endl;
		for(Edge & it : all)
			std::cout << it;
		std::cout<<std::endl << "Borders:"<<std::endl;
		for(Edge & it : bord)
			std::cout << it;
	}
	void print_edges(std::string const & fall, std::string const & fbord, std::string const & finternal);
	//void print_polpt(){
		//for(auto it=vap.begin(); it!=vap.end();++it)
			//(*it)->print_pt();
	//}
};//END CLASS GRID

#endif
