#include <iostream>
#include "parameters.hpp"
std::ostream & operator << (std::ostream & out,const parameters & p)
{
  out<<"PARAMETER VALUES:"<<"\n";
  out<<"itermax= "<<p.itermax<<"\n";
  out<<"toler= "<<p.toler<<"\n";
  out<<"L= "<<p.L<<"\n";
  out<<"a1= "<<p.a1<<"\n";
  out<<"a2= "<<p.a2<<"\n";
  out<<"To= "<<p.To<<"\n";
  out<<"Te= "<<p.Te<<"\n";
  out<<"k= "<<p.k<<"\n";
  out<<"hc= "<<p.hc<<"\n";
  out<<"M= "<<p.M<<"\n";
  out<<"fileout= "<<p.fileout<<"\n";
  out<<"howres= "<<p.howres<<"\n";
  out<<"norm= "<<p.norm<<"\n";
  out<<"method= "<<p.method<<"\n";
  out<<"nstat= "<<p.nstat<<"\n";
  out<<"Ti= "<<p.Ti<<"\n";
  out<<"TT= "<<p.TT<<"\n";
  out<<"MT= "<<p.MT<<"\n\n";
  return out;
}
