#include "GetPot.hpp"
#include "readParameters.hpp"
#include <fstream>
#include <string>
parameters readParameters(std::string const & filename,bool verbose)
{
  // Parameter default constructor fills it with the defaults values
  parameters defaults;
  // checks if file exixts and is readable
  std::ifstream check(filename); 
  if(!check)
    {
      std::cerr<<"ERROR: Parameter file "<<filename<<" does not exist"<<std::endl;
      std::cerr<<"Reverting to default values."<<std::endl;
      if(verbose) std::cout<<defaults;
      check.close();
      return defaults;
    }
  else
    check.close();

  GetPot ifile(filename.c_str());
  parameters values;
  // Read parameters from getpot ddata base
  values.itermax=ifile("itermax",defaults.itermax);
  values.toler=ifile("toler",defaults.toler);
  values.L=ifile("L",defaults.L);
  values.a1=ifile("a1",defaults.a1);
  values.a2=ifile("a2",defaults.a2);
  values.To=ifile("To",defaults.To);
  values.Te=ifile("Te",defaults.Te);
  values.k=ifile("k",defaults.k);
  values.hc=ifile("hc",defaults.hc);
  values.M=ifile("M",defaults.M);
  values.fileout=ifile("fileout",defaults.fileout.c_str());
  std::string tmp=ifile("howres","both");
  int ttmp=tmp.compare("tofile");
  if(ttmp<0) values.howres=HowRes::BOTH;
  else if(ttmp==0) values.howres=HowRes::TOFILE;
  else values.howres=HowRes::TOPLOT;
  tmp.clear();
  tmp=ifile("norm","vect");
  ttmp=tmp.compare("l2");
  if(ttmp<0) values.norm=Norm::H1;
  else if(ttmp==0) values.norm=Norm::L2;
  else values.norm=Norm::VECT;
  values.method=ifile("method",defaults.method);
  values.nstat=ifile("nstat",defaults.nstat);
  values.Ti=ifile("Ti",defaults.Ti);
  values.TT=ifile("TT",defaults.TT);
  values.MT=ifile("MT",defaults.MT);
  if(verbose)
    {
      std::cout<<"PARAMETER VALUES IN GETPOT FILE"<<"\n";
      ifile.print();
      std::cout<<std::endl;
      std::cout<<"ACTUAL VALUES"<<"\n"<<values;
    }
  return values;
}
