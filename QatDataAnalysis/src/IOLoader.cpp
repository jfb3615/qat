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

#define STR_VALUE(arg) #arg
#define STR_NAME(name) STR_VALUE(name)

#include "QatDataAnalysis/IOLoader.h"
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <vector>
class IOLoader::Clockwork {
public:
  
  
  
  typedef void * (*CreationMethod) ();
  
  std::string _loadFileName;
  
  std::vector<std::string>                 _knownShort  ={"RootDriver", "HDF5Driver"};
#ifdef __APPLE__
  std::vector<std::string>                 _knownFull   ={"libQoot.dylib", "libQHDF5.dylib"};
  std::map<std::string, std::string>       _fullName    ={{"RootDriver", "libQoot.dylib"},
						          {"HDF5Driver", "libQHDF5.dylib"}};
  
  std::map<std::string, std::string>       _shortName   ={{"libQoot.dylib", "RootDriver"},
							  {"libQHDF5.dylib", "HDF5Driver"}};
  
#else
  std::vector<std::string>                 _knownFull   ={"libQoot.so", "libQHDF5.so"};
  std::map<std::string, std::string>       _fullName    ={{"RootDriver", "libQoot.so"},
						          {"HDF5Driver", "libQHDF5.so"}};
  
  std::map<std::string, std::string>       _shortName   ={{"libQoot.so", "RootDriver"},
							  {"libQHDF5.so", "HDF5Driver"}};
  
#endif
  std::map<std::string, CreationMethod>    _create;
  std::map<std::string, const IODriver *>  _driver;

 std::map<std::string, std::string>       _driverLocation;
  
  void doLoad(const std::string &thisDir, const std::string & name) {  
    
    std::string  path = thisDir + std::string("/") + name;
    std::string shortName=_shortName[name];
    
    // .. and their is only one driver on the list..
    if (shortName!="") {
      if (_create.find(shortName)==_create.end()) {
	
	std::string createFunctionName;
	createFunctionName=shortName+"Create";
	
	void *handle = dlopen(path.c_str(),RTLD_LAZY);
	if (!handle) std::cerr << dlerror() << std::endl;
	
	CreationMethod createMethod;
	union { CreationMethod func; void* data; } sym;
	
	sym.data= dlsym(handle, createFunctionName.c_str());
	createMethod = sym.func;
	_create[shortName]=createMethod;
	if (!createMethod) {
	  std::cout << shortName << " load failed" << std::endl;
	}
      }
    }
  }
  
  
  void loadDrivers() {

    // Preprocessor:  we are passing the install directory to this
    // class. Drivers will be loaded from this install directory.
    // However in some cases "linux" is part of the install directory
    // name and that may be defined in a macro to be 1. So if that is
    // the case we temporarily undef it. 
#ifdef linux
#define waslinux linux
#undef linux
    std::string standardPlaces = STR_NAME(QATLIBDIR);
#define linux waslinux
#else 
    std::string standardPlaces = STR_NAME(QATLIBDIR);
#endif
    char *ldlibpath = strdup(standardPlaces.c_str());
    
    if (ldlibpath) {
      char *thisDir=strtok(ldlibpath,(char *)":");
      while (thisDir) {
	DIR *directory = opendir(thisDir);
	if (directory) {
	  dirent * entry = readdir(directory);
	  while (entry) {
	    
	    std::string  name = entry->d_name;
	    for (std::string driverName: _knownFull) {
	      if (name==driverName) {
		if (_driverLocation.find(_shortName[driverName])==_driverLocation.end()) {
		  _driverLocation[_shortName[driverName]]=thisDir;
		}
	      }
	    }
	    entry = readdir(directory);
	  }
	  closedir(directory);
	  if (_driverLocation.size()==2) break;
	}
	thisDir=strtok(NULL, (char *) ":");
      }
    }
  }
};

IOLoader::IOLoader():c(new Clockwork()) {
  c->loadDrivers();
}

IOLoader::~IOLoader() {
  delete c;
}



const IODriver *IOLoader::ioDriver(const std::string & shortName ) const {


  if (c->_driverLocation.find(shortName)==c->_driverLocation.end()) {
    std::cout << shortName << " load failed" << std::endl;
    return NULL;
  }
  
  c->doLoad(c->_driverLocation[shortName], c->_fullName[shortName]);

  
  std::map<std::string, const IODriver *>::iterator d = c->_driver.find(shortName);
  if (d==c->_driver.end()) {
    std::map<std::string, Clockwork::CreationMethod>::iterator m = c->_create.find(shortName);
    if (m!=c->_create.end()) {
      Clockwork::CreationMethod createMethod = (*m).second;
      IODriver *driver = (IODriver *) (*createMethod) ();
      c->_driver[shortName] =driver;
      return driver;
    }
    else {
      return NULL;
    }
  }
  else 
    return (*d).second;
  
}
