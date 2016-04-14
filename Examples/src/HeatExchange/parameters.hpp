#ifndef HH_Parameters_HH
#define HH_Parameters_HH
#include <iosfwd>
#include <string>
enum HowRes {
	TOFILE,
	TOPLOT,
	BOTH
};
enum Norm {
	VECT,
	L2,
	H1
};

struct parameters
{
  //! max number of iteration for Gauss-Siedel
  int   itermax;
  //! Tolerance for stopping criterion
  double  toler;
  //! Bar length
   double L;
  //! First longitudinal dimension
  double a1;
 //! Second longitudinal dimension
  double a2;
  //! Dirichlet condition
  double To;
  //! External temperature 
  double Te;
  //! Conductivity
  double k;
  //! Convection coefficient
  double hc;
  //! Number of elements
  int M;
  //! File where the result will be stored
  std::string fileout;
  //! How to show the result
  HowRes howres;
  //! Norm to use to compute the residual
  Norm norm;
  //! Method to solve the linear sys: 0=iterative (Gauss-Seidel),
  //1=direct(Thomas)
  unsigned int method;
  //! Constructor takes default values
  parameters():
    itermax(1000000),
    toler(1e-8),
    L(40.),
    a1(4.),
    a2(50.),
    To(46.),
    Te(20.),
    k(0.164),
    hc(1.e-6*200.),
    M(100),
	fileout("result.dat"),
	howres(HowRes::BOTH),
	norm(Norm::VECT),
	method(0)
  {}
};
//! Prints parameters
std::ostream & operator << (std::ostream &,const parameters &);
#endif
