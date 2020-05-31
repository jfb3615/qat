#include "QatDataAnalysis/OptParse.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main (int argc, char **argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0] + " -o Output [-v]"; 
  if (argc==1) {
    std::cout << usage << std::endl;
    exit(0);
  }
  HIOZeroToOne hio;
  try {
    hio.optParse(argc,argv);
  }
  catch (std::exception & e) {
    std::cerr << "Error " << e.what()  << std::endl;
    std::cerr << usage    << std::endl << std::endl;
    exit(0);
  }
  if (argc!=1) {      
    std::cerr << "Unrecognized option: " << argv[1] <<std::endl;
    exit(0);
  }
  
  // -------------------------------------------------:
  return 1;

}

