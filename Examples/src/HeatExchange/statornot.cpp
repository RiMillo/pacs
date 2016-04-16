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
#include "algothomas.h"

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
	thomas(maindiag,updiag,lowdiag,b,theta);

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
	//const auto& hres=param.howres;	// How to show the result
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
	std::vector<double> theta(M+1), theta_old(M+1,Ti-Te); 
	
	cout<<"Result file: "<<fout<<endl;
	ofstream f(fout);
	f<<"t=0.0"<<endl;
	for(int m = 0; m<= M; m++)
	{
		// \t writes a tab 
		f<< m*h*L<<"\t"<<Te*(1.+theta_old[m])<<endl;
	}

	//Building the tri diag matrix via 3 vectors
	std::vector<double>	lowdiag(M,-dt),			// Lower diag
						maindiag(M+1,1+dt*(2.+h*h*act)),	// Main diag
						updiag(M,-dt);			// Upper diag
	//Setting the right coefficients for the boundaries
	maindiag[0]=1.0;
	updiag[0]=0.0;
	maindiag[M]=1.0;
	lowdiag[M-1]=-1.0;
	//Fixing the right coefficient
	theta_old[0]=(To-Te)/Te;

	// Time loop
	for(int t=1;t<MT;++t)
	{
		
		thomas(maindiag,updiag,lowdiag,theta_old,theta);

		// writing results with format
		// t= t^k
		// x_i u_h(x_i)^k  
		
		f<<"t="<<dt*t<<endl;
		for(int m = 0; m<= M; m++)
		{
			// \t writes a tab 
			f<< m*h*L<<"\t"<<Te*(1.+theta[m])<<endl;
		}
		//theta_old.clear();
		//Updating
		theta_old=theta;
		theta_old[M]=0.0;
	}//end FOR time
	f.close();
	//if(hres==HowRes::TOPLOT || hres==HowRes::BOTH) {
		//Gnuplot gp;
		//std::vector<double> coor(M+1);
		//std::vector<double> sol(M+1);
		//std::vector<double> exact(M+1);

		//for(int m = 0; m<= M; m++)
			//std::tie(coor[m],sol[m],exact[m])=
			  //std::make_tuple(m*h*L,Te*(1.+theta[m]),thetaa[m]);
		 // //Using temporary files (another nice use of tie)
		//gp<<"plot"<<gp.file1d(std::tie(coor,sol))<<
			//"w lp title 'uh',"<< gp.file1d(std::tie(coor,exact))<<
			//"w l title 'uex'"<<std::endl;
	//}
	return;
}

