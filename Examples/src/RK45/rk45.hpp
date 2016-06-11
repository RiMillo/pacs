#ifndef HH_RK45INTEGRATOR_HH
#define HH_RK45INTEGRATOR_HH
#include <functional>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
namespace ODE
{
  //! Defailt max number of iteration
  constexpr std::size_t MAX_STEPS=10000; 
  //! Single step of RK
  /*!
    for the solution of \f$ dy/dt=f(y(y),t)

    The routine returns the error as the absoute value of the
    difference between the fifth order and fourth order
    approximation. An optimal step size could be computed by
    postprocessing the error with the formula
    \f[
    h_{opt}=(\frac{\epsilon h}{2error})^{1/4}
    ]\f
    where \epsilon is the error control tolerance.
    
    @param dy  the right hand side function :f(t,y)
    @param y0  initial value
    @param h   time step size
    @param error the estimated error
    @return the computed value for y
  */
  template<class Emb>
  class RK_step {
  public:
	RK_step() = default;
    double step(
			std::function<double (double const &, double const &)> const & f,
			double const & y0,
			double const & t0,
			double const & h, 
			double & error);
  };

  //! Implementation
  template<class Emb>
  double RK_step<Emb>::step(
		  std::function<double (double const &, double const &)> const & f,
		  double const & y0,
		  double const & t0,
		  double const & h, 
		  double & error) 
  {
	std::vector<double> F(Emb::A.size()+1);
	F[0] = h * f(t0,y0);
	for (size_t i = 1; i < F.size(); ++i) {
		double c(0.), y(y0);
		for (size_t j = 0; j < i; ++j) {
			c += Emb::A[i-1][j];
			y += Emb::A[i-1][j] * F[j];
		}//END FOR 
		F[i] = h * f(t0 + c * h, y);
	}
   /* //Debug*/
	//for (auto i = F.begin(); i != F.end(); ++i) {
		//std::cout << *i;
	//}//END FOR i
	//std::cout << std::endl;

	double ybl(y0), yl(y0);
	for (size_t i = 0; i < Emb::b.size(); ++i) {
		ybl += F[i]*Emb::btilde[i];
		yl  += F[i]*Emb::b[i];
	}
	error = std::abs(yl - ybl);
	return yl;
  }


  //! Adaptive RK
  /*!  Integrates /f[ dy/dt(t)=f(t,y(t)) \quad t \in (t0,T] \quad
    y(t0)=y0 /f] controlling the error at t=T I avoid recursion
    because too inefficient.  I do not use the formula for the optimal
    time step. I adopt a simple bisection procedure: the step is
    halved or doubled according to a test on the error. A maximal time step size
    is given to avoid too large time steps, which may cause overlooking variation of f
    with small support.

    @param dy the right hand side f(t,y)
    @param t0 initial time
    @param T final time
    @param y0 initial value
    @param h_initial initial time step size
    @param h_max maximal time step size allowed
    @param final_error Desired error at t=T
    @param status: 0=Ok 1=initial step too small; 2 number of steps exceeded; 
                   3 hmin reached, cannot guarantee error
    @return the solution as pairs t,y
   */
  template<class Emb>
  class RK {
  public:
	RK() = default;
	std::vector<std::pair<double,double>> 
    solve(std::function<double (double const &, double const &)> const & dy,
	 double const & t0,
	 double const & T,
	 double const & y0,
	 double const & h_initial, 
	 double const & h_max, 
	 double const & final_error,
	 int & status,
	 std::size_t const & maxSteps=MAX_STEPS);
  private:
	RK_step<Emb> _step;
  };
//! Implementation
  template<class Emb>
  std::vector<std::pair<double,double>> 
  RK<Emb>::solve(std::function<double (double const &, double const &)> const & dy,
	 double const & t0,
	 double const & T,
	 double const & y0,
	 double const & h_initial, 
	 double const & h_max, 
	 double const & final_error,
	 int & status,
	 std::size_t const & maxSteps)
  {
    status=0;
    const std::size_t maxReduction=maxSteps;
    // parameters for decreasing/increasing time step
    double const c1=1.0;
    // I need to have a sufficient decrease of the local error
    // to allow time step coarsening
    double const c2=1./64.;

    double length=T-t0;
    //! Make sure that h allows to reach T
    std::size_t initialNSteps=std::max(static_cast<size_t>(1),static_cast<size_t>(length/h_initial));
    double h=length/initialNSteps;
    // To avoid underflow we need in any case to limit the time step to a positive number
    // Here I allow h to become 128 time smaller than that giving the maximal number of steps
    double h_min = length/(128*maxSteps);
    // SOme counters
    std::size_t stepsCounter(0);
    // Initial data
    double time(t0);
    double y(y0);
    double errorPerTimeStep=final_error/initialNSteps;
    if (initialNSteps>=maxSteps) throw std::runtime_error("RK45: initial time step h too small!");
    std::vector<std::pair<double,double>> solution;
    solution.emplace_back(std::make_pair(t0,y0));
    double localError;
    double newy;
    while (time<T && stepsCounter <maxSteps)
      {
	//Do a step
	//adjust h if needed for the last step
	if (time + h > T) h = T-time;
	newy = _step.step(dy,y,time,h,localError);
	while (h> h_min && localError > c1*errorPerTimeStep)
	  {
	    // half time step
	    h /=2;
	    errorPerTimeStep /=2;
	    newy = _step.step(dy,y,time,h,localError);
		//std::cout << localError << std::endl;
	  }
	if (localError>errorPerTimeStep)status=1;
	//! advance
	y = newy;
	time +=h;
	++stepsCounter;
	solution.emplace_back(std::make_pair(time,y));
	//! check if we reached end
	if(localError<c2*errorPerTimeStep && h<h_max)
	  {
	    // Double step
	    h *=2;
	    errorPerTimeStep *=2;
	  }
      }
    //handle exceptions
    if(stepsCounter>=maxSteps && time < T)
      {
	status=2;
	throw std::runtime_error("RK45: Max number of time steps exceeded");
      }
    return solution;
  }
  
  typedef std::vector<std::vector<double> > butcher;

  //! RK45 parameters
  class RK45 {
  public:
	RK45() = default;
    static butcher					A;
	static std::vector<double>		btilde;
	static std::vector<double>		b;
  };
  butcher RK45::A = { 
    {1./4.},
    {3./32.,			9./32.},
	{1932./2197.,	-7200./2197.,	7296./2197.},
	{439./216.,		-8.,			3680./513.,		-845./4104.},
	{-8./27.,		2.,				-3544./2565.,	1859./4104.,	-11./40.}
  };
  std::vector<double> RK45::btilde = {
    25./216., 0., 1408./2565.,  2197./4104,    -1./5.,  0.
  };
  std::vector<double> RK45::b = {
    16./135., 0., 6656./12825., 28561./56430., -9./50., 2./55.
  };

  //! RK23 parameters
  class RK23 {
  public:
	RK23() = default;
	static butcher					A;
	static std::vector<double>		btilde;
	static std::vector<double>		b;
  };
  butcher RK23::A = {
    {1./2.},
	{0.,	 3./4.},
	{2./9.,	 1./3.,		4./9.}
  };
  std::vector<double> RK23::btilde = {
    2./9.,    1./3.,    4./9.,		0.
  };
  std::vector<double> RK23::b = {
    7./24.,	  1./4.,	1./3.,		1./8.
  };
}// end namespace
#endif
