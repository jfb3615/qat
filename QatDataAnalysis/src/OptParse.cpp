//---------------------------------------------------------------------------//
// !!                                                                     !! //
//                                                                           //
//  Copyright (C) 2016 Joe Boudreau                                          //
//                                                                           //
//  This file is part of the QAT Toolkit for computational science           //
//                                                                           //
//  QAT is free software: you can redistribute it and/or modify              //
//  it under the terms of the GNU Lesser General Public License as           //
//  published by the Free Software Foundation, either version 3 of           //
//  the License, or (at your option) any later version.                      //
//                                                                           //
//  QAT is distributed in the hope that it will be useful,                   //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU Lesser General Public License for more details.                      //
//                                                                           //
//  You should have received a copy of the GNU Lesser General Public         //
//  License along with QAT.  If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//---------------------------------------------------------------------------//

#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "QatDataAnalysis/OptParse.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatDataAnalysis/IOLoader.h"
#include "QatDataAnalysis/IODriver.h"
#include "QatDataAnalysis/Query.h"


inline std::string getDriver(std::string driverName) {
  // Rules:
  if (driverName!="") {
    return driverName;
  }
  else {
    char *dn=getenv("QAT_IO_DRIVER");
    if (dn) {
      return std::string(dn);
    }
    else {
      return "HDF5Driver";
    }
  }
}



class HIOZeroToOne::Clockwork {
public:
  IOLoader        loader;
  const IODriver *driver;
};

HIOZeroToOne::HIOZeroToOne(const std::string & driver):output(NULL), verbose(false), c(new Clockwork) {
  c->driver = c->loader.ioDriver(getDriver(driver));
  if (!c->driver) throw std::runtime_error("Warning, could not open a driver. Possible installation problem");
}

HIOZeroToOne::~HIOZeroToOne() {
  if (output) c->driver->write(output);
  if (output) c->driver->close(output);
  delete c->driver;
  delete c;
}

void HIOZeroToOne::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    if (std::string(argv[i])=="-o") {
      i++;
      if (i<argc) {
	if (output) {
	  throw std::runtime_error("HIOZeroToOne two output files specified" );
	}
	else {
	  output    = c->driver->newManager (argv[i]);
	  std::copy (argv+i+1, argv+argc, argv+i-1);
	  argc -=2;
	  i    -=2;
	}
      }
      else {
	throw std::runtime_error("Cannot parse output file" );
      }
    }
    else if (std::string(argv[i])=="-v") {
      verbose=true;
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
  
  }
  if (!output) throw std::runtime_error("HIOZeroToOne options parse error" );
}


class HIOOneToOne::Clockwork {
public:
  IOLoader        loader;
  const IODriver *driver;
};

HIOOneToOne::HIOOneToOne(const std::string & driver):input(NULL),output(NULL), verbose(false), c(new Clockwork) {
  c->driver = c->loader.ioDriver(getDriver(driver));
  if (!c->driver) throw std::runtime_error("Warning, could not open a driver. Possible installation problem");
}

HIOOneToOne::~HIOOneToOne() {
  if (output) c->driver->write(output);
  if (output) c->driver->close(output);
  if (input) c->driver->close(input);
  delete c->driver;
  delete c;
}

void HIOOneToOne::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    if (std::string(argv[i])=="-o") {
      i++;
      if (i<argc) {
	if (output) {
	  throw std::runtime_error("HIOOneToOne two output files specified" );
	}
	else {
	  output    = c->driver->newManager (argv[i]);
	  std::copy (argv+i+1, argv+argc, argv+i-1);
	  argc -=2;
	  i    -=2;
	}
      }
      else {
	throw std::runtime_error("Cannot parse output file" );
      }
    }
    else if (std::string(argv[i])=="-i") {
      i++;
      if (i<argc) {
	if (input) {
	  throw std::runtime_error("HIOOneToOne two input files specified" );
	}
	else {
	  input    = c->driver->openManager (argv[i]);
	  std::copy (argv+i+1, argv+argc, argv+i-1);
	  argc -=2;
	  i    -=2;
	}
      }
      else {
	throw std::runtime_error("Cannot parse input file" );
      }
    }
    else if (std::string(argv[i])=="-v") {
      verbose=true;
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
  
  }
  if (!output) throw std::runtime_error("HIOOneToOne options parse error" );
  if (!input)  throw std::runtime_error("HIOOneToOne no input file" );
}

class HIOOneToZero::Clockwork {
public:
  IOLoader        loader;
  const IODriver *driver;
};

HIOOneToZero::HIOOneToZero(const std::string & driver):input(NULL), verbose(false), c(new Clockwork) {
  c->driver = c->loader.ioDriver(getDriver(driver));
  if (!c->driver) throw std::runtime_error("Warning, could not open a root driver. Possible installation problem");
}

HIOOneToZero::~HIOOneToZero() {
  if (input) c->driver->close(input);
  delete c->driver;
  delete c;
}

void HIOOneToZero::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    if (std::string(argv[i])=="-i") {
      i++;
      if (i<argc) {
	if (input) {
	  throw std::runtime_error("HIOOneToZero two input files specified" );
	}
	else {
	  input    = c->driver->openManager (argv[i]);
	  std::copy (argv+i+1, argv+argc, argv+i-1);
	  argc -=2;
	  i    -=2;
	}
      }
      else {
	throw std::runtime_error("Cannot parse input file" );
      }
    }
    else if (std::string(argv[i])=="-v") {
      verbose=true;
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
  
  }
  if (!input)  throw std::runtime_error("HIOOneToZero no input file" );
}


class HIONToOne::Clockwork {
public:
  IOLoader        loader;
  const IODriver *driver;
};

HIONToOne::HIONToOne(const std::string & driver):output(NULL), verbose(false), c(new Clockwork) {
  c->driver = c->loader.ioDriver(getDriver(driver));
  if (!c->driver) throw std::runtime_error("Warning, could not open a root driver. Possible installation problem");
}

HIONToOne::~HIONToOne() {
  if (output) c->driver->write(output);
  if (output) c->driver->close(output);
  for (unsigned int i=0;i<input.size();i++) c->driver->close(input[i]);
  delete c->driver;
  delete c;
}

void HIONToOne::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    if (std::string(argv[i])=="-o") {
      i++;
      if (i<argc) {
	if (output) {
	  throw std::runtime_error("HIONToOne two output files specified" );
	}
	else {
	  output    = c->driver->newManager (argv[i]);
	  std::copy (argv+i+1, argv+argc, argv+i-1);
	  argc -=2;
	  i    -=2;
	}
      }
      else {
	throw std::runtime_error("Cannot parse output file" );
      }
    }
    else if (std::string(argv[i])=="-v") {
      verbose=true;
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
    else {
      if (std::string(argv[i]).find('=')==std::string(argv[i]).npos) {
	const HistogramManager *manager=c->driver->openManager(argv[i]);
	if (manager) input.push_back(manager);
	std::copy(argv+i+1, argv+argc, argv+i);
	argc -= 1;
	i    -= 1;
      }
    }
  }
  if (!input.size()) throw std::runtime_error("HIONToOne no input file" );
  if (!output) throw std::runtime_error("HIONToOne options parse error" );
}

class HIONToZero::Clockwork {
public:
  IOLoader        loader;
  const IODriver *driver;
};

HIONToZero::HIONToZero(const std::string & driver):verbose(false), c(new Clockwork) {
  c->driver = c->loader.ioDriver(getDriver(driver));
  if (!c->driver) throw std::runtime_error("Warning, could not open a root driver. Possible installation problem");
}

HIONToZero::~HIONToZero() {
  for (unsigned int i=0;i<input.size();i++) c->driver->close(input[i]);
  delete c->driver;
  delete c;
}

void HIONToZero::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    if (std::string(argv[i])=="-v") {
      verbose=true;
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
    else {
      if (std::string(argv[i]).find('=')==std::string(argv[i]).npos) {
	const HistogramManager *manager=c->driver->openManager(argv[i]);
	if (manager) input.push_back(manager);
	std::copy(argv+i+1, argv+argc, argv+i);
	argc -= 1;
	i    -= 1;
      }
    }
  }
  if (!input.size()) throw std::runtime_error("HIONToZero no input file" );
}




NumericInput::NumericInput() :c(new Clockwork()) {
}

NumericInput::~NumericInput() {
  delete c;
}


std::string NumericInput::usage() const {
  std::ostringstream stream;
  stream << "Parameter usage" << std::endl;
  for (unsigned int i = 0; i< c->inputData.size();i++) {
    stream << c->inputData[i].name << " " << c->inputData[i].doc << '\t' << c->inputData[i].value << std::endl;
  }  
  return stream.str();

}
void NumericInput::optParse(int & argc, char ** & argv) {
  for (int i=1; i<argc;i++) {
    std::string thisArg(argv[i]);
    if (thisArg.find("=")!=thisArg.npos) {

      std::string modifier=thisArg;

      // Parse modifier
      size_t pos = modifier.find('=');
      if (pos!=modifier.npos) {
	std::string variable = modifier.substr(0,pos);
	std::string value    = modifier.substr(pos+1,modifier.npos);
	std::istringstream   valueStream(value),iValueStream(value);
	double val;
	int    ival;
	valueStream >> val;
	iValueStream >> ival;
	if (!valueStream || !iValueStream) {
	  throw std::runtime_error("Cannot modify parameter "+ modifier);	
	}
	bool found=false;
	for (unsigned int i=0;i<c->inputData.size();i++) {
	  std::string trimmedVar= (c->inputData[i].name.substr(0,c->inputData[i].name.find(' ')));
	  if (trimmedVar==variable) {
	    c->inputData[i].value=val;
	    c->inputData[i].iValue=ival;
	    found=true;
	  }
	}
	if (!found) {
	  throw std::runtime_error("Cannot modify parameter "+ modifier);
	}
      }
      else {
	throw std::runtime_error("Cannot modify parameter "+ modifier);
      }
      std::copy(argv+i+1, argv+argc, argv+i);
      argc -= 1;
      i    -= 1;
    }
  }
}

