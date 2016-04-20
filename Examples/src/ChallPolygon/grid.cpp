#include "grid.hpp"

#include <string>
#include <fstream>
#include <memory>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>

using namespace std; 

Grid::Grid(string const & file) {
	ifstream    f(file);
	if (f) {
		string      tmpline;
		getline(f, tmpline);  
		array<unsigned int,2> N;
		string::size_type sz;
		// 1 - Getting points and polygons numbers
		N[0]=stoi(tmpline,&sz);
		N[1]=stoi(tmpline.substr(sz));
		
		// 2 - Building point vector
		vpt.reserve(N[0]);
		//auto const bb=vpt.begin();
		double x,y;
		//unsigned int pos;
		for(unsigned int npt=0; npt<N[0];++npt) {
			assert(getline(f, tmpline));
			//Get the position (in case they won't be ordered
			//Discard it for now
			stoi(tmpline,&sz);
			//Get the coordinates
			tmpline=tmpline.substr(sz);
			x = stod(tmpline,&sz);
			tmpline=tmpline.substr(sz);
			y = stod(tmpline,&sz);
			//Inserting the new element
			vpt.emplace_back(x,y);
			tmpline.clear();
		}//END FOR POINT
		
		// 3- Building polygon vector
		vap.reserve(N[1]);
		//auto const bbb=vap.begin();
		unsigned int shape;
		//vector<Geometry::Point2D> vert;	//2.1
		vector<unsigned int> vert;	//2.2
		for(unsigned int npt=0; npt<N[1];++npt) {
			assert(getline(f, tmpline));
			//Get the position (in case they won't be ordered
			//Discard it for now
			stoi(tmpline,&sz);
			//Get the shape
			tmpline=tmpline.substr(sz);
			shape = stoi(tmpline,&sz);
			unsigned int pp;
			//Switch on shape
			switch(shape) {
			case 0:
				//Trinagle
				vert.reserve(3);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<3;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Triangle(vert));	//2.1
				vap.emplace_back(new Geometry::Triangle(vert,&vpt));	//2.2
				break;
			case 1:
				//Square
				vert.reserve(4);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<4;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Square(vert));	//2.1
				vap.emplace_back(new Geometry::Square(vert,&vpt));	//2.2

				break;
			default:
				//Generic
				vert.reserve(6);
				tmpline=tmpline.substr(sz);
				while(tmpline.size()>0){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Polygon(vert));	//2.1
				vap.emplace_back(new Geometry::Polygon(vert,&vpt));	//2.2
				break;
			}//END SWITCH
			vert.clear();
		}//END FOR POLYGON
		f.close();
	} else {throw("File not found"); exit(EXIT_FAILURE);}    //END IF F
}//END CONSTRUCTOR
void Grid::read_set(string const & file) {
	this->clear();
	ifstream    f(file);
	if (f) {
		string      tmpline;
		getline(f, tmpline);  
		array<unsigned int,2> N;
		string::size_type sz;
		// 1 - Getting points and polygons numbers
		N[0]=stoi(tmpline,&sz);
		N[1]=stoi(tmpline.substr(sz));
		
		// 2 - Building point vector
		vpt.resize(N[0]);
		double x,y;
		unsigned int pos;
		for(unsigned int npt=0; npt<N[0];++npt) {
			assert(getline(f, tmpline));
			//Get the position (in case they won't be ordered
			//Discard it for now
			pos = stoi(tmpline,&sz);
			//Get the coordinates
			tmpline=tmpline.substr(sz);
			x = stod(tmpline,&sz);
			tmpline=tmpline.substr(sz);
			y = stod(tmpline,&sz);
			//Inserting the new element
			vpt[pos]=Geometry::Point2D(x,y);
			tmpline.clear();
		}//END FOR POINT
		
		// 3- Building polygon vector
		vap.reserve(N[1]);
		unsigned int shape;
		//vector<Geometry::Point2D> vert;	//2.1
		vector<unsigned int> vert;	//2.2
		for(unsigned int npt=0; npt<N[1];++npt) {
			assert(getline(f, tmpline));
			//Get the position (in case they won't be ordered
			//Discard it for now
			stoi(tmpline,&sz);
			//Get the shape
			tmpline=tmpline.substr(sz);
			shape = stoi(tmpline,&sz);
			unsigned int pp;
			//Switch on shape
			switch(shape) {
			case 0:
				//Trinagle
				vert.reserve(3);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<3;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Triangle(vert));	//2.1
				vap.emplace_back(new Geometry::Triangle(vert,&vpt));	//2.2
				break;
			case 1:
				//Square
				vert.reserve(4);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<4;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Square(vert));	//2.1
				vap.emplace_back(new Geometry::Square(vert,&vpt));	//2.2

				break;
			default:
				//Generic
				vert.reserve(6);
				tmpline=tmpline.substr(sz);
				while(tmpline.size()>0){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
				}
				//vap.emplace_back(new Geometry::Polygon(vert));	//2.1
				vap.emplace_back(new Geometry::Polygon(vert,&vpt));	//2.2
				break;
			}//END SWITCH
			vert.clear();
		}//END FOR POLYGON
		f.close();
	} else {throw("File not found"); exit(EXIT_FAILURE);}    //END IF F
}//END CONSTRUCTOR

double Grid::sum_area(){
	double res(0.0);
	//for(unsigned int i=0; i<vap.size();++i)
		//res += abs((vap[i])->area());
	for(auto it=vap.begin();it!=vap.end();++it)
		res += abs((*it)->area());
	return res;
}//END SUM_AREA
