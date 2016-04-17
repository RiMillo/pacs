#include "statornot.hpp"

#include <iostream> // input output
#include <cmath> // (for sqrt)
#include <vector>
#include <tuple>
#include <algorithm>
#include "readParameters.hpp"
#include "GetPot.hpp"
#include "gnuplot-iostream.hpp"// interface with gnuplot
#include "cmpnorm.hpp"
//#include "algothomas.h"
#include "trimat.hpp"

void stationary(parameters const & param) 
{
	// Transfer parameters to local variables
	// I use references to save memory (not really an issue here, it is just
	// to show a possible  use of references)
	//const double& toler=param.toler;   // Tolerance for stopping criterion
	// Here I use auto (remember that you need const and & if you want constant references)
	const auto& L= param.L;  // Bar length
	const auto& a1=param.a1; // First longitudinal dimension
	const auto& a2=param.a2; //  Second longitudinal dimension
	const auto& To=param.To; // Dirichlet condition
	const auto& Te=param.Te; // External temperature (Centigrades)
	const auto& k=param.k;  // Thermal conductivity
	const auto& hc=param.hc; // Convection coefficient
	const auto&    M=param.M; // Number of grid elements
	const auto& fout=param.fileout;  //Where result will be stored
	const auto& hres=param.howres;  //How to show the result
	//const auto& nrm =param.norm;  //Norm to use

	//! Precomputed coefficient for adimensional form of equation
	const auto act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

	// mesh size
	const auto h=1./M;
	  
	// Solution vector
	std::vector<double> theta(M+1);
	  
	//Building the tri diag matrix via 3 vectors
	vector<double>	lowdiag(M,-1.0),			// Lower diag
					maindiag(M+1,2.+h*h*act),	// Main diag
					updiag(M,-1.0);			// Upper diag
	//Setting the right coefficients for the boundaries
	maindiag[0]=1.0;
	updiag[0]=0.0;
	maindiag[M]=1.0;
	//building the knowns vector
	vector<double> b(M+1,0.0);
	//Fixing the right coefficient
	b[0]=(To-Te)/Te;
	TriMat mat(maindiag,updiag,lowdiag);
	mat.thomas(b,theta);

	// Analitic solution

	vector<double> thetaa(M+1);
	for(int m=0;m <= M;m++)
	   thetaa[m]=Te+(To-Te)*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

	// writing results with format
	// x_i u_h(x_i) u(x_i) and lauch gnuplot 

	if(hres==HowRes::TOFILE || hres==HowRes::BOTH) {
		cout<<"Result file: "<<fout<<endl;
		ofstream f(fout);
		for(int m = 0; m<= M; m++)
		{
			// \t writes a tab 
			f<<m*h*L<<"\t"<<Te*(1.+theta[m])<<"\t"<<thetaa[m]<<endl;
		}
		f.close();
	}
	if(hres==HowRes::TOPLOT || hres==HowRes::BOTH) {
		Gnuplot gp;
		std::vector<double> coor(M+1);
		std::vector<double> sol(M+1);
		std::vector<double> exact(M+1);

		for(int m = 0; m<= M; m++)
			std::tie(coor[m],sol[m],exact[m])=
			  std::make_tuple(m*h*L,Te*(1.+theta[m]),thetaa[m]);
		// Using temporary files (another nice use of tie)
		gp<<"plot"<<gp.file1d(std::tie(coor,sol))<<
			"w lp title 'uh',"<< gp.file1d(std::tie(coor,exact))<<
			"w l title 'uex'"<<std::endl;
	}
	return;
}

void NON_stationary(parameters const & param) 
{
	// Transfer parameters to local variables
	// I use references to save memory (not really an issue here, it is just
	// to show a possible  use of references)
	//const double& toler=param.toler;   // Tolerance for stopping criterion
	// Here I use auto (remember that you need const and & if you want constant references)
	const auto& L= param.L;			// Bar length
	const auto& a1=param.a1;		// First longitudinal dimension
	const auto& a2=param.a2;		// Second longitudinal dimension
	const auto& To=param.To;		// Dirichlet condition
	const auto& Te=param.Te;		// External temperature (Centigrades)
	const auto& k=param.k;			// Thermal conductivity
	const auto& hc=param.hc;		// Convection coefficient
	const auto& M=param.M;			// Number of grid elements
	const auto& fout=param.fileout; // Where result will be stored
	const auto& hres=param.howres;	// How to show the result
	//const auto& nrm=param.norm;		// Norm to use
	const auto& Ti=param.Ti;		// Initial temperature (constant on all the bar)
	const auto& TT=param.TT;		// Time limit
	const auto& MT=param.MT;		// Number of time steps
	//! Precomputed coefficient for adimensional form of equation
	const auto act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

	// mesh size
	const auto h=1./M;
	const auto dt=TT/(MT-1);
	  
	// Solution vector
	std::vector<double> theta(M+1), theta_old(M+1,Ti-Te), b(M+1); 
	std::vector<double> tCoor((M+1)*MT), xCoor((M+1)*MT), val((M+1)*MT);

	for(unsigned int m=0;m<M+1;++m)
		std::tie(tCoor[m],xCoor[m],val[m]) = std::make_tuple(0.0,h*m*L,Ti);
	
	//cout<<"Result file: "<<fout<<endl;
	//ofstream f(fout);
	//f<<"t=0.0"<<endl;
	//for(int m = 0; m<= M; m++)
	//{
		//f<< m*h*L<<"\t"<<Te*(1.+theta_old[m])<<endl;
	//}

	//! Building the matrix used to compute the rhs of the lin-sys from the
	//! previous time step sol
	std::vector<double>	lowdiag(M,+dt/2),			// Lower diag
						maindiag(M+1,1-dt/2*(2.+h*h*act)),	// Main diag
						updiag(M,+dt/2);			// Upper diag
	//Setting the right coefficients for the boundaries
	TriMat r_mat(maindiag,updiag,lowdiag);

	//! Building the tridiag matrix of the lin-sys.
	std::fill(maindiag.begin(),maindiag.end(),1+dt/2*(2.+h*h*act));
	std::fill(lowdiag.begin(),lowdiag.end(),-dt/2);
	std::fill(updiag.begin(),updiag.end(),-dt/2);
	//! Setting some coeffs for the boundary conditions
	maindiag[0]=1.0;
	updiag[0]=0.0;
	maindiag[M]=1.0;
	lowdiag[M-1]=-1.0;
	TriMat mat(maindiag,updiag,lowdiag);

	theta_old[0]=(To-Te)/Te;
	// Time loop
	for(int t=1;t<MT;++t)
	{
		r_mat.prod_vect(theta_old,b);
		//Fixing the right coefficient
		b[0]=(To-Te)/Te;
		b[M]=0.0;
	
		mat.thomas(b,theta);

		// writing results with format
		// t= t^k
		// x_i u_h(x_i)^k  
		
		//f<<"t="<<dt*t<<endl;
		//for(int m = 0; m<= M; m++)
		//{
			 ////\t writes a tab 
			//f<< m*h*L<<"\t"<<Te*(1.+theta[m])<<endl;
		//}
		for(unsigned int m=0;m<M+1;++m)
			std::tie(tCoor[t*(M+1)+m],xCoor[t*(M+1)+m],val[t*(M+1)+m]) =
				std::make_tuple(t*dt,h*m*L,Te*(1.+theta[m]));
		//theta_old.clear();
		//Updating
		theta_old=theta;
	}//end FOR time
	//f.close();

	// Output
	if(hres==HowRes::TOFILE || hres==HowRes::BOTH){
		std::cout<<"Results file: "<<fout<<std::endl;
		ofstream f(fout);
		for(unsigned int m=0; m<tCoor.size();++m)
			f<<tCoor[m]<<"\t"<<xCoor[m]<<"\t"<<val[m]<<std::endl;
		f.close();
	}
	if(hres==HowRes::TOPLOT || hres==HowRes::BOTH) {
		Gnuplot gp;
		gp<< "set pm3d" << std::endl;
		gp<< "set style data lines" <<std::endl;
		gp<< "unset key" <<std::endl;
		gp<< "set title 'NON stationary solution of Heat Eq'"<<std::endl;
		gp<< "set xlabel 'Space'"<<std::endl;
		gp<< "set ylabel 'Time'"<<std::endl;
		gp<< "set zlabel 'Temperature' rotate parallel"<<std::endl;
		gp<< "set dgrid3d " <<MT <<","<< M+1<<std::endl;
		gp<< "splot" <<gp.file1d(std::tie(xCoor,tCoor,val))<< "pal" <<std::endl;
	}
	return;
}

