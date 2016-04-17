#include "trimat.hpp"

#include <cassert>

using namespace std;


void TriMat::thomas(vector<double> const & rhs,
					vector<double> & res) {
	assert(sz==rhs.size());
	vector<double> c(sz), d(sz);

	c[0] = upd[0]/maind[0];
	d[0] = rhs[0]/maind[0];
	for(unsigned int i=1; i < sz-1; i++) {
		c[i] = upd[i]/(maind[i]-lowd[i-1]*c[i-1]);
		d[i] = ( rhs[i]-lowd[i-1]*d[i-1] ) /
				( maind[i] - lowd[i-1]*c[i-1]);
	}
	d[sz-1] = ( rhs[sz-1]-lowd[sz-2]*d[sz-2] ) /
				( maind[sz-1] - lowd[sz-2]*c[sz-2]);

	res[sz-1] = d[sz-1];
	for(unsigned int i=sz-1; i-->0;)
		res[i] = d[i] - c[i]*res[i+1];

	return;
}

void TriMat::prod_vect(vector<double> & rhs, vector<double> & res) {
	assert(sz==rhs.size());
	if(res.size()!=sz)
		res.resize(sz);
	res[0]=maind[0]*rhs[0]+upd[0]*rhs[1];
	for(unsigned int i=1;i<sz-1;++i) {
		res[i] = lowd[i-1]*rhs[i-1] + 
					maind[i]*rhs[i] +
					upd[i]*rhs[i+1];
	}
	res[sz-1] = lowd[sz-2]*rhs[sz-2] + maind[sz-1]*rhs[sz-1]; 
}

void operator<<(ostream & o, TriMat & m) {
	o<<"["<<m.maind[0]<<",\t"<<m.upd[0];
	for(unsigned int i=2;i<m.sz;++i)
		o<<",\t0.0";
	o<<";\n";
	for(unsigned int i=1; i<m.sz-1;++i) {
		unsigned int j(0);
		o<<" ";
		for(;j<i-1;++j)
			o<<"0.0,\t";
		o<<m.lowd[i-1]<<",\t"<<m.maind[i]<<",\t"<<m.upd[i];
		j+=3;
		for(;j<m.sz;++j)
			o<<",\t0.0";
		o<<";\n";
	}
	o<<" ";
	for(unsigned int j=0;j<m.sz-2;++j)
		o<<"0.0,\t";
	o<<m.lowd[m.sz-2]<<",\t"<<m.maind[m.sz-1]<<"]\n";
}

