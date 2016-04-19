#ifndef HH_GRID_HH
#define HH_GRID_HH

#include "Polygon.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

using namespace Geometry;

class Grid
{
private:
	std::vector<Point2D> vpt;
	std::vector<std::shared_ptr<AbstractPolygon>> vap;
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

};//END CLASS GRID

#endif
