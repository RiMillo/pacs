#ifndef ALGOTHOMAS_H
#define ALGOTHOMAS_H

#include <vector>

using namespace std;

//Thomas' algo for resolving a tridiagonal linear sys
//another modif
void thomas(vector<double> const & p_diag, vector<double> const & u_diag, vector<double> const & l_diag, vector<double> const & src, vector<double> & res) {
   unsigned int N(p_diag.size());

   vector<double> c(N), d(N);

   c[0] = u_diag[0]/p_diag[0];
   d[0] = src[0]/p_diag[0];
   for(unsigned int i=1; i < N-1; i++) {
       c[i] = u_diag[i]/(p_diag[i]-l_diag[i-1]*c[i-1]);
       d[i] = ( src[i]-l_diag[i-1]*d[i-1] ) /
               ( p_diag[i] - l_diag[i-1]*c[i-1]);
   }
   d[N-1] = ( src[N-1]-l_diag[N-2]*d[N-2] ) /
           ( p_diag[N-1] - l_diag[N-2]*c[N-2]);

   res[N-1] = d[N-1];
   for(unsigned int i=N-1; i-->0;)
       res[i] = d[i] - c[i]*res[i+1];

}

#endif // ALGOTHOMAS_H
