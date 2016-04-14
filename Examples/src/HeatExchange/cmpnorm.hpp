#ifndef HH_Cmpnorm_HH
#define HH_Cmpnorm_HH
#include <vector>
#include "parameters.hpp"

double normL2 (std::vector<double> const & f, double const h); 

double normH1 (std::vector<double> const & f, double const h); 

double normvect (std::vector<double> const & f);

double choosenorm (std::vector<double> const & f, Norm const & nr, double const h);
#endif
