#include "grid.hpp"

#include <string>
#include <fstream>
#include <memory>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <set>
#include <utility>
#include <vector>
#include <string>

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
		
		// 3- Building polygon vector & temporary edges
		vap.reserve(N[1]);
		//auto const bbb=vap.begin();
		unsigned int shape;
		//vector<Geometry::Point2D> vert;	//2.1
		vector<unsigned int> vert;	//2.2
		//Utilities for build vectors edges
		unsigned int prev, fst;
		set<Edge> tmp_all, tmp_bord;
		set<Edge>::iterator iter;
		bool				insert;
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
			{
				//Trinagle
				vert.reserve(3);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<3;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
					if(i==0) {
						fst=pp;
						prev=pp;
						continue;
					}
					Edge e(prev,pp);
					std::tie(iter,insert)=tmp_all.insert(e);
					if(insert)	tmp_bord.insert(e);
					else		tmp_bord.erase(e);
					if(i==2) {
						e=Edge(fst,pp);
						std::tie(iter,insert)=tmp_all.insert(e);
						if(insert)	tmp_bord.insert(e);
						else		tmp_bord.erase(e);
					}
					prev=pp;
				}
				//vap.emplace_back(new Geometry::Triangle(vert));	//2.1
				vap.emplace_back(new Geometry::Triangle(vert,&vpt));	//2.2
				break;
			}
			case 1:
			{
				//Square
				vert.reserve(4);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<4;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
					if(i==0) {
						fst=pp;
						prev=pp;
						continue;
					}
					Edge e(prev,pp);
					std::tie(iter,insert)=tmp_all.insert(e);
					if(insert)	tmp_bord.insert(e);
					else		tmp_bord.erase(e);
					if(i==3) {
						e=Edge(fst,pp);
						std::tie(iter,insert)=tmp_all.insert(e);
						if(insert)	tmp_bord.insert(e);
						else		tmp_bord.erase(e);
					}
					prev=pp;

				}
				//vap.emplace_back(new Geometry::Square(vert));	//2.1
				vap.emplace_back(new Geometry::Square(vert,&vpt));	//2.2

				break;
			}
			default:
			{
				//Generic
				vert.reserve(6);
				tmpline=tmpline.substr(sz);
				pp=stoi(tmpline,&sz);
				fst=pp;
				prev=pp;
				vert.push_back(pp);
				tmpline=tmpline.substr(sz);
				while(tmpline.size()>0){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
					Edge e(prev,pp);
					std::tie(iter,insert)=tmp_all.insert(e);
					if(insert)	tmp_bord.insert(e);
					else		tmp_bord.erase(e);
					if(!(tmpline.size()>0)) {
						//Last
						e=Edge(fst,pp);
						std::tie(iter,insert)=tmp_all.insert(e);
						if(insert)	tmp_bord.insert(e);
						else		tmp_bord.erase(e);
						break;
					}
					prev=pp;
				}
				//vap.emplace_back(new Geometry::Polygon(vert));	//2.1
				vap.emplace_back(new Geometry::Polygon(vert,&vpt));	//2.2
				break;
			}
			}//END SWITCH
			vert.clear();
		}//END FOR POLYGON
		all.resize(tmp_all.size());
		copy(tmp_all.begin(),tmp_all.end(),all.begin());
		bord.resize(tmp_bord.size());
		copy(tmp_bord.begin(),tmp_bord.end(),bord.begin());
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
		
		// 3- Building polygon vector & temporary edges
		vap.reserve(N[1]);
		//auto const bbb=vap.begin();
		unsigned int shape;
		//vector<Geometry::Point2D> vert;	//2.1
		vector<unsigned int> vert;	//2.2
		//Utilities for build vectors edges
		unsigned int prev, fst;
		set<Edge> tmp_all, tmp_bord;
		set<Edge>::iterator iter;
		bool				insert;
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
			{
				//Trinagle
				vert.reserve(3);
				tmpline=tmpline.substr(sz);
				//1
				pp=stoi(tmpline,&sz);
				vert.push_back(pp);
				//Updates
				fst=pp; prev=pp; tmpline=tmpline.substr(sz);
				//2
				pp=stoi(tmpline,&sz);
				vert.push_back(pp);
				Edge e(prev,pp);
				std::tie(iter,insert)=tmp_all.insert(e);
				if(insert)	tmp_bord.insert(e);
				else		tmp_bord.erase(e);
				//Updates
				prev=pp; tmpline=tmpline.substr(sz);
				//3
				pp=stoi(tmpline,&sz);
				vert.push_back(pp);
				e=Edge(prev,pp);
				std::tie(iter,insert)=tmp_all.insert(e);
				if(insert)	tmp_bord.insert(e);
				else		tmp_bord.erase(e);
				e=Edge(fst,pp);
				std::tie(iter,insert)=tmp_all.insert(e);
				if(insert)	tmp_bord.insert(e);
				else		tmp_bord.erase(e);
				//vap.emplace_back(new Geometry::Triangle(vert));	//2.1
				vap.emplace_back(new Geometry::Triangle(vert,&vpt));	//2.2
				break;
			}
			case 1:
			{
				//Square
				vert.reserve(4);
				tmpline=tmpline.substr(sz);
				for(unsigned int i=0; i<4;++i){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
					if(i==0) {
						fst=pp;
						prev=pp;
						continue;
					}
					Edge e(prev,pp);
					std::tie(iter,insert)=tmp_all.insert(e);
					if(insert)	tmp_bord.insert(e);
					else		tmp_bord.erase(e);
					if(i==3) {
						e=Edge(fst,pp);
						std::tie(iter,insert)=tmp_all.insert(e);
						if(insert)	tmp_bord.insert(e);
						else		tmp_bord.erase(e);
					}
					prev=pp;

				}//END FOR 
				//vap.emplace_back(new Geometry::Square(vert));	//2.1
				vap.emplace_back(new Geometry::Square(vert,&vpt));	//2.2

				break;
			}
			default:
			{
				//Generic
				vert.reserve(6);
				tmpline=tmpline.substr(sz);
				pp=stoi(tmpline,&sz);
				fst=pp;
				prev=pp;
				vert.push_back(pp);
				tmpline=tmpline.substr(sz);
				while(tmpline.size()>0){
					pp=stoi(tmpline,&sz);
					//vert.push_back(vpt[pp]);	//2.1
					vert.push_back(pp);	//2.2
					tmpline=tmpline.substr(sz);
					Edge e(prev,pp);
					std::tie(iter,insert)=tmp_all.insert(e);
					if(insert)	tmp_bord.insert(e);
					else		tmp_bord.erase(e);
					if(!(tmpline.size()>0)) {
						//Last
						e=Edge(fst,pp);
						std::tie(iter,insert)=tmp_all.insert(e);
						if(insert)	tmp_bord.insert(e);
						else		tmp_bord.erase(e);
						break;
					}
					prev=pp;
				}//END WHILE
				//vap.emplace_back(new Geometry::Polygon(vert));	//2.1
				vap.emplace_back(new Geometry::Polygon(vert,&vpt));	//2.2
				break;
			}
			}//END SWITCH
			vert.clear();
		}//END FOR POLYGON
		all.resize(tmp_all.size());
		copy(tmp_all.begin(),tmp_all.end(),all.begin());
		bord.resize(tmp_bord.size());
		copy(tmp_bord.begin(),tmp_bord.end(),bord.begin());

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

void Grid::print_edges(string const & fall, string const & fbord, string const & finternal){
	ofstream fa(fall),fb(fbord),fi(finternal);
	set<unsigned int> sall, sbord;
	//Building and printing all the points belonging at least to an edge
	for(Edge const & e : all) {
		sall.insert(e.a);
		sall.insert(e.b);
	}
	for(unsigned int i : sall)
		fa << i << "\t";
	fa.close();
	//Building and printing all the points belonging to a boundary edge
	for(Edge const & e : bord) {
		sbord.insert(e.a);
		sbord.insert(e.b);
	}
	for(unsigned int i : sbord)
		fb << i << "\t";
	fb.close();
	//Building and printing all the internal points
	vector<unsigned int> vint(sall.size());
	vector<unsigned int>::iterator it =
		set_difference(sall.begin(), sall.end(),
						sbord.begin(), sbord.end(),
						vint.begin());
	vint.resize(it-vint.begin());
	for(unsigned int i : vint)
		fi << i << "\t";
	fi.close();
}
