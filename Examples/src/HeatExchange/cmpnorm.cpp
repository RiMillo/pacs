#include "cmpnorm.hpp"
#include <vector>

double normL2 (std::vector<double> const & f, double const h) {
	double res(0.0);
	auto it=f.begin();
	res +=(*it)*(*it)*h/2;
	for(;it!=f.end()-1;it++)
		res +=(*it)*(*it)*h;
	res += f.back()*f.back()*h/2;
	return res;
}

double normH1 (std::vector<double> const & f, double const h) {
	unsigned int M=f.size();
	std::vector<double> diff(M);
	diff[0]=(f[1]-f[2])/h;
	for(unsigned int m=1; m<M-1;m++)
		diff[m]=(f[m-1]-f[m+1])/2/h;
	diff[M-1]=(f[M-1]-f[M-2])/h;
	return normL2(f,h)+normL2(diff,h);
}

double normvect (std::vector<double> const & f) {
	double res(0.0);
	for(auto it=f.begin();it!=f.end();it++)
		res += (*it)*(*it);
	return res;
}

double choosenorm (std::vector<double> const & f, Norm const & nr, double const h) {
	switch(nr) {
	case Norm::L2:
		return normL2(f,h);
		break;
	case Norm::H1:
		return normH1(f,h);
		break;
	default:
		return normvect(f);
		break;
	}
}


