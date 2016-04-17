#ifndef HH_TriMat_HH
#define HH_TriMat_HH

// Class for a square tridiagonal matrices

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

class TriMat
{
protected:
	//! Size
	unsigned int sz; 
	//! Main diag
	vector<double> maind;
	//! Upper and lower diags
	vector<double> upd, lowd;
public:
	//! Ctors
	TriMat(unsigned int const n) : sz(n) 
	{
		maind.resize(n);
		upd.resize(n-1);
		lowd.resize(n-1);
	}
	TriMat(vector<double> const & pd,
			vector<double> const & ud,
			vector<double> const & ld)
	{
		unsigned int n=pd.size();
		assert((n==ud.size()+1) && (n==ld.size()+1));
		sz=n;
		maind=pd;
		upd=ud;
		lowd=ld;
	}

	//! Gets
	unsigned int	 get_size()		{return sz;}
	vector<double> & get_maind()	{return maind;}
	vector<double> & get_upd()		{return upd;}
	vector<double> & get_lowd()		{return lowd;}
	inline double operator() (unsigned int const i, unsigned int const j);
	//! Sets
	inline void set_elem (unsigned int const i, unsigned int const j, double const val); 
	void set_maind(vector<double> const & v) {
		assert(sz==v.size());
		maind=v;
		return;
	}
	void set_lowd(vector<double> const & v) {
		assert(sz-1==v.size());
		lowd=v;
		return;
	}
	void set_upd(vector<double> const & v) {
		assert(sz-1==v.size());
		upd=v;
		return;
	}

	void thomas(vector<double> const & rhs, vector<double> & res);
	void prod_vect(vector<double> & rhs, vector<double> & res);
	friend void operator<<(ostream & o, TriMat & m);

};//END TriMat

double TriMat::operator() (unsigned int const i, unsigned int const j) {
	assert(i<sz && j<sz);
	int d(i-j);
	switch(d){
	case (-1):
		return upd[i];
		break;
	case 0:
		return maind[i];
		break;
	case 1:
		return lowd[j];
		break;
	default:
		return 0.0;
	}
}

void TriMat::set_elem (unsigned int const i, unsigned int const j, double const val) {
	assert(i<sz && j<sz);
	assert(abs(i-j)<2);
	int d(i-j);
	switch(d){
	case (-1):
		upd[i]=val;
		break;
	case 0:
		maind[i]=val;
		break;
	case 1:
		lowd[j]=val;
		break;
	default:
		break;
	}
	return;
}

#endif
