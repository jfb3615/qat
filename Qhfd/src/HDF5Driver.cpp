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

#include "QatDataAnalysis/IODriver.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatDataAnalysis/Tuple.h"

#include "hdf5.h"

#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unistd.h>
#include <memory>
#include <stdexcept>
#include <cstring>
#define MAX_NAME 1024

//* -----------------declarations -------------------*//

class HDF5Driver: public IODriver {
 
public:

  // Constructor:
  HDF5Driver();

  // Destructor:
  ~HDF5Driver();

  // New Manager:
  virtual HistogramManager *newManager(const std::string & filename)  const;
  
  // OpenManager:
  virtual const HistogramManager *openManager(const std::string & filename) const;

  // Writes the Histograms out.
  virtual void close (const HistogramManager *mgr) const;

  // Closes the file:
  virtual void write (HistogramManager *mgr) const;
    
 private:


  void read(HistogramManager * mgr) const;
  
  mutable std::map<std::string, HistogramManager *> _mgrMap;
  mutable std::map<std::string, hid_t             > _istrMap;
  mutable std::map<std::string, hid_t             > _ostrMap;

  mutable hid_t _id;
  mutable hid_t _od;
};



class HDF5IOStore: public TupleStore {

public:

  enum TCode {
    Double_c=0,
    Int_c,
    UInt_c,
    Float_c
  };
    
  // Constructor:
  HDF5IOStore(hid_t dataset);

  // For input:
  virtual TupleConstLink operator [](size_t i) const;

  // Get the size:
  virtual size_t size() const {
    return nTuples;
  }

  AttributeListConstLink attributeList() const { 
    return internalTable->attributeList();
  }

private:

  // Destructor:
  virtual ~HDF5IOStore();

  

  hid_t                          _dataset;
  hid_t                          _space;
  hid_t                          _type;
  hid_t                          _memtype;
  hid_t                          _SBspace;
  
  Table                          *internalTable;
  TupleLink                      internalTuple;
  std::vector<std::string>       nList;
  size_t                         nAttributes;
  size_t                         nTuples;
  char                           *buffer;

  friend class ImaginaryFriend;

}; 


TupleConstLink HDF5IOStore::operator [] (size_t i) const {

  if (i>=nTuples) return NULL;

  hsize_t start[]={i};
  //hsize_t stride[]={1};
  hsize_t count[]={1};
  //hsize_t block[]={1};

  H5Sselect_hyperslab (_space, H5S_SELECT_SET, start, NULL, count, NULL);
  H5Dread (_dataset, _memtype, _SBspace, _space, H5P_DEFAULT,buffer);
  

  internalTuple->uncache();
  ValueList & vList = const_cast <ValueList &> (internalTuple->valueList());
  size_t length=0;
  for (size_t a=0;a<internalTuple->attributeList()->size();a++) {
    if ((*internalTuple->attributeList())[a].type()==Attribute::DOUBLE) {
      memcpy((void *) vList[a].asCharStar(), buffer+length, sizeof(double));
      length+=sizeof(double);
    }
    else if ((*internalTuple->attributeList())[a].type()==Attribute::FLOAT){
      memcpy((void *) vList[a].asCharStar(), buffer+length, sizeof(float));
      length+=sizeof(float);
    }
    else if ((*internalTuple->attributeList())[a].type()==Attribute::INT){
      memcpy((void *) vList[a].asCharStar(), buffer+length, sizeof(int));
      length+=sizeof(int);
    }
    else if ((*internalTuple->attributeList())[a].type()==Attribute::UINT) {
      memcpy((void *) vList[a].asCharStar(), buffer+length, sizeof(unsigned int));
      length+=sizeof(unsigned int);
    }
    else if ((*internalTuple->attributeList())[a].type()==Attribute::UINT64) {
      memcpy((void *) vList[a].asCharStar(), buffer+length, sizeof(size_t));
      length+=sizeof(size_t);
    }
  }
  
  return internalTuple;

}


HDF5IOStore::~HDF5IOStore() {
  delete [] buffer;
  delete internalTable;
  H5Tclose(_type);
  H5Tclose(_memtype);
  H5Sclose(_space);
  H5Dclose(_dataset);
  H5Sclose(_SBspace);

}

HDF5IOStore::HDF5IOStore(hid_t dataset):
_dataset(dataset),
_space(0),
_type(0),
_memtype(0),
internalTable(NULL),
internalTuple(NULL),
nAttributes(0),
nTuples(0),
buffer(0)
{
  //
  // 1. Get the dimensions
  //
  hsize_t dims[1]={0};
  _space = H5Dget_space (_dataset);
  size_t ndims = H5Sget_simple_extent_dims (_space, dims, NULL);
  if (ndims!=1) {
    throw std::runtime_error("Error in ntuple format (ndims!=1)");
  }
  else {
    nTuples=dims[0];
  }

  //
  // 2. Get the datatype
  //
  _type=H5Dget_type(_dataset);
  nAttributes=H5Tget_nmembers(_type);
    
  //
  // 2. Make an "internal tuple" 
  //
  internalTable       = new Table("Internal Use Only");
  size_t length=0;
  for (unsigned int i=0;i<nAttributes;i++) {
    std::string name(H5Tget_member_name(_type, i ));
    hid_t typeName=H5Tget_member_type(_type, i );
    if (H5Tequal(typeName,H5T_NATIVE_FLOAT)) {
       length+=sizeof(float);
    }
    else if (H5Tequal(typeName,H5T_NATIVE_DOUBLE)) {
      length+=sizeof(double); 
    }
    else if (H5Tequal(typeName,H5T_NATIVE_INT)) {
      length+=sizeof(int);
    }
    // End  addition
    else if (H5Tequal(typeName,H5T_NATIVE_UINT)) {
      length+=sizeof(unsigned int);
    }
    else if (H5Tequal(typeName,H5T_NATIVE_ULONG)) {
      length+=sizeof(size_t);
    }
    else {
      std::cerr << "[HDF5Driver:  Unknown type " << typeName << " "  << " will be ignored on input]" << std::endl;
    }
    H5Tclose(typeName);
  }


  _memtype = H5Tcreate(H5T_COMPOUND,length);
  size_t offset=0;
  for (unsigned int i=0;i<nAttributes;i++) {
    std::string name(H5Tget_member_name(_type, i ));
    hid_t typeName=H5Tget_member_type(_type, i );
    if (H5Tequal(typeName,H5T_NATIVE_FLOAT)) {
       internalTable->add(name,float(0));
       H5Tinsert (_memtype, name.c_str(),offset, H5T_NATIVE_FLOAT);
       offset+=sizeof(float);
    }
    else if (H5Tequal(typeName,H5T_NATIVE_DOUBLE)) {
      internalTable->add(name,double(0));
      H5Tinsert (_memtype, name.c_str(),offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    else if (H5Tequal(typeName,H5T_NATIVE_INT)) {
      internalTable->add(name,int(0));
      H5Tinsert (_memtype, name.c_str(),offset, H5T_NATIVE_INT);
      offset+=sizeof(int);    
    }
    // End  addition
    else if (H5Tequal(typeName,H5T_NATIVE_UINT)) {
      internalTable->add(name,size_t(0));
      H5Tinsert (_memtype, name.c_str(),offset, H5T_NATIVE_UINT);
      offset+=sizeof(unsigned int);    
    }
    else if (H5Tequal(typeName,H5T_NATIVE_ULONG)) {
      internalTable->add(name,size_t(0));
      H5Tinsert (_memtype, name.c_str(),offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);    
    }
    else {
      std::cerr << "[HDF5Driver:  Unknown type " << typeName << " "  << " will be ignored on input]" << std::endl;
    }
    nList.push_back(name);
    H5Tclose(typeName);
  }
  buffer = new char[length];
  hsize_t slabsize[]={1};
  _SBspace = H5Screate_simple (1, slabsize, NULL);

  // End of header.  Now start the data:
  internalTuple       = (TupleLink) internalTable->capture();
    
}


// Creation routine:
extern "C" void *HDF5DriverCreate();







//*----------------implementation------------------*//


extern "C" void *HDF5DriverCreate() {
  return new HDF5Driver();
} 


HDF5Driver::HDF5Driver():_id(0),_od(0) {
  std::cerr << "[HDF5Driver:  HELLO.]" << std::endl;
}


HDF5Driver::~HDF5Driver() {
}

const HistogramManager *HDF5Driver::openManager(const std::string & filename)  const {
  // See if this file is indeed a root file:
  {
    std::ifstream ifile(filename.c_str());
    char buf[4]="   ";
    char ofx;
    ifile >> ofx;
    ifile >> buf[0] >> buf[1] >> buf[2];
    if (std::string(buf)=="HDF") {
    }
    else {
      std::cerr << "[HDF5Driver: File NOT OK:" << filename << "]" << std::endl;
      return NULL;
    }
  }
  _ostrMap[filename]=H5Fopen(filename.c_str(),H5F_ACC_RDONLY,H5P_DEFAULT);
   
  HistogramManager *mgr = new HistogramManager(filename);
  _mgrMap[filename]=mgr;
  read(mgr);
  _id=0;
  std::cerr << "[HDF5Driver:  Open Histogram file " << filename << "]" << std::endl;
  
  return mgr;

}


HistogramManager *HDF5Driver::newManager(const std::string & filename)  const {

 
  // Create a new file using the default properties.
   
  hid_t stream = H5Fcreate (filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  _istrMap[filename]=stream;

  HistogramManager *mgr = new HistogramManager();
  _mgrMap[filename]=mgr;
                                                                                                           
  std::cerr << "[HDF5Driver:  New Histogram file " << filename << "]" << std::endl;

  return mgr;

}

// Writes the Histograms out.
void HDF5Driver::close (const HistogramManager *mgr) const {
  // Find the file name:
  std::map<std::string, HistogramManager *>::iterator 
    begin=_mgrMap.begin(),
    end=_mgrMap.end(),m=begin;
  std::string filename;
  while (m!=end) {
    if ((*m).second==mgr) {
      filename = (*m).first;
      break;
    }
    m++;
  }
  
  if (filename!="") {
    {
      hid_t file = _ostrMap[filename];
      if (file) H5Fclose(file);
    }
    {
      hid_t file = _istrMap[filename];
      if (file) H5Fclose(file);
    }
    {
      std::map<std::string, HistogramManager *>::iterator d=_mgrMap.find(filename);
      if (d!=_mgrMap.end()) _mgrMap.erase(d);
    }
    {
      std::map<std::string, hid_t>::iterator d=_ostrMap.find(filename);
      if (d!=_ostrMap.end()) _ostrMap.erase(d);
    }
    {
      std::map<std::string, hid_t>::iterator d=_istrMap.find(filename);
      if (d!=_istrMap.end()) _istrMap.erase(d);
    }
  }
  
  
}

// Closes the file:
void HDF5Driver::write (HistogramManager *mgr) const {

  // Find the output file
  // Find the output stream for this:
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  if (!_od) {
    for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
      if (mgr==(*m).second) {
	name = (*m).first;
	_od= _istrMap[name];
	break;
      }
    }
  }
  if (!_od) return;

  // Write out all of the ntuples:

  HistogramManager::DConstIterator d;
  for (d=mgr->beginDir();d!=mgr->endDir();d++) {
    hid_t prev=_od;
    _od = H5Gcreate(_od, (*d)->name().c_str(), H5P_DEFAULT, H5P_DEFAULT,H5P_DEFAULT);
    write (*d);
    H5Gclose(_od);
    _od=prev;
  }

  // Write out all ntuples:
  HistogramManager::TConstIterator t;
  for (t=mgr->beginTable();t!=mgr->endTable();t++) {
    std::string prefix="T:";
    size_t length=0;
    for (size_t a=0;a<(**t).numAttributes();a++) {
      if ((**t).attribute(a).type()==Attribute::DOUBLE) length+=sizeof(double);
      else if ((**t).attribute(a).type()==Attribute::FLOAT)     length+=sizeof(float);
      else if ((**t).attribute(a).type()==Attribute::INT)       length+=sizeof(int);
      else if ((**t).attribute(a).type()==Attribute::UINT)      length+=sizeof(unsigned int);
      else if ((**t).attribute(a).type()==Attribute::UINT64)    length+=sizeof(size_t);
    }

    hid_t memtype = H5Tcreate(H5T_COMPOUND,length);
    hid_t filetype = H5Tcreate (H5T_COMPOUND, length);

    {
      size_t offset=0;
      for (size_t a=0;a<(**t).numAttributes();a++) {
	if ((**t).attribute(a).type()==Attribute::DOUBLE) 
	  {
	    H5Tinsert (filetype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_DOUBLE);
	    H5Tinsert (memtype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_DOUBLE);
	    offset+=sizeof(double);
	  }
	else if ((**t).attribute(a).type()==Attribute::FLOAT)     
	  {
	    H5Tinsert (memtype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_FLOAT);
	    H5Tinsert (filetype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_FLOAT);
	    offset+=sizeof(float);
	  }
	else if ((**t).attribute(a).type()==Attribute::INT)       
	  {
	    H5Tinsert (memtype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_INT);
	    H5Tinsert (filetype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_INT);
	    offset+=sizeof(int);
	  }
	else if ((**t).attribute(a).type()==Attribute::UINT)      
	  {
	    H5Tinsert (memtype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_UINT);
	    H5Tinsert (filetype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_UINT);
	    offset+=sizeof(unsigned int);
	  }
	else if ((**t).attribute(a).type()==Attribute::UINT64)      
	  {
	    H5Tinsert (memtype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_ULONG);
	    H5Tinsert (filetype, (**t).attribute(a).name().c_str(),offset, H5T_NATIVE_ULONG);
	    offset+=sizeof(size_t);
	  }
	else 
	  {
	    throw std::logic_error("Logic error in HDF5Driver");
	  }
      }
    }
    hsize_t     dims[1];
    dims[0] = (**t).numTuples();; 
    hid_t space_id = H5Screate_simple(1, dims, NULL);
    hid_t dataset_id = H5Dcreate(_od, (prefix+(**t).name()).c_str(), filetype, space_id,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    
    hsize_t hCount[]={1};
    hsize_t hOffset[]={0};
    hsize_t slabsize[]={1};
    hid_t SBspace = H5Screate_simple (1, slabsize, NULL);


    for (size_t n = 0; n<(**t).numTuples(); n++) {
      TupleConstLink thisTuple = (**t)[n];
      if (!thisTuple) break;
      char *buff=new char[length];
      size_t pos=0;
      for (size_t v=0;v<thisTuple->valueList().size();v++) {
	double bt=0;
	if ((**t).attribute(v).type()==Attribute::DOUBLE)bt=sizeof(double); 
	if ((**t).attribute(v).type()==Attribute::FLOAT) bt=sizeof(float); 
	if ((**t).attribute(v).type()==Attribute::INT)   bt=sizeof(int); 
	if ((**t).attribute(v).type()==Attribute::UINT)  bt=sizeof(unsigned int); 
	if ((**t).attribute(v).type()==Attribute::UINT64)  bt=sizeof(size_t); 
	memcpy(buff+pos,thisTuple->valueList()[v].asCharStar(),bt);
	pos+=bt;
      }

      hid_t XTspace = H5Dget_space (dataset_id);
      H5Sselect_hyperslab (XTspace, H5S_SELECT_SET, hOffset, NULL,hCount, NULL);
      

      H5Dwrite (dataset_id, memtype, SBspace, XTspace, H5P_DEFAULT, buff); //(const void *) &thisTuple->valueList()[0]);
      hOffset[0]++;
      H5Sclose (XTspace);
      delete [] buff;
    }


    H5Dclose(dataset_id);
    H5Sclose(space_id);
    H5Sclose(SBspace);
    H5Tclose(filetype);
    H5Tclose(memtype);
    
  }

  for (HistogramManager::H1ConstIterator h=mgr->beginH1();h!=mgr->endH1();h++) {
    std::string prefix="1:";
    const Hist1D::Clockwork *c=(**h).clockwork();
    size_t length=0;
    length+=sizeof(c->nBins);
    length+=sizeof(c->min);
    length+=sizeof(c->max);
    length+=sizeof(c->binWidth);              
    length+=sizeof(c->overflow);              
    length+=sizeof(c->underflow);             
    length+=sizeof(c->entries);               
    length+=sizeof(c->xW);                    
    length+=sizeof(c->xxW);                   
    length+=sizeof(c->sumWeight);             
    length+=sizeof(c->minContents);           
    length+=sizeof(c->maxContents);
    size_t headLength=length;
    length+=sizeof(c->contents[0])*c->contents.size();
    length+=sizeof(c->errors[0])*c->errors.size();
    hid_t memtype = H5Tcreate(H5T_COMPOUND,length);
    hid_t filetype = H5Tcreate (H5T_COMPOUND, length);
    size_t offset=0;
    {
      H5Tinsert (filetype, "nBins",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "nBins",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "min",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "min",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "max",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "max",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "binWidth",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "binWidth",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "overflow",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "overflow",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "underflow",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "underflow",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "entries",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "entries",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "xW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "xW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "xxW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "xxW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "sumWeight",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "sumWeight",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "minContents",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "minContents",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "maxContents",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "maxContents",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    hsize_t arrayDims[]={c->contents.size()};
    hid_t arr_id= H5Tarray_create (H5T_NATIVE_DOUBLE,1,arrayDims);

    {
      H5Tinsert (filetype, "contents",offset, arr_id);
      H5Tinsert (memtype,  "contents",offset, arr_id);
      offset+=sizeof(c->contents[0])*c->contents.size();
    }
    {
      H5Tinsert (filetype, "errors",offset, arr_id);
      H5Tinsert (memtype,  "errors",offset, arr_id);
      offset+=sizeof(c->errors[0])*c->errors.size();
    }
    hsize_t     dims[]={1}; 
    hid_t space_id = H5Screate_simple(1, dims, NULL);
    hid_t dataset_id = H5Dcreate(_od, (prefix+c->name).c_str(), filetype, space_id,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    char *buff=new char[length];
    memcpy(buff, &c->nBins,headLength);
    memcpy(buff+headLength,&c->contents[0],sizeof(c->contents[0])*c->contents.size());
    memcpy(buff+headLength+sizeof(c->contents[0])*c->contents.size(),&c->errors[0],sizeof(c->errors[0])*c->errors.size());
   
    H5Dwrite (dataset_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buff);
    delete [] buff;
    H5Dclose(dataset_id);
    H5Sclose(space_id);
    H5Tclose(memtype);
    H5Tclose(filetype);
    H5Tclose(arr_id);
  }

  for (HistogramManager::H2ConstIterator h=mgr->beginH2();h!=mgr->endH2();h++) {
    std::string prefix="2:";
    const Hist2D::Clockwork *c=(**h).clockwork();
    size_t length=0;
    length+=sizeof(c->nBinsX);
    length+=sizeof(c->nBinsY);
    length+=sizeof(c->minX);
    length+=sizeof(c->maxX);
    length+=sizeof(c->minY);
    length+=sizeof(c->maxY);
    length+=sizeof(c->binWidthX);              
    length+=sizeof(c->binWidthY);              
    length+=sizeof(c->overUnderFlow);              
    length+=sizeof(c->overflow);             
    length+=sizeof(c->xW);                    
    length+=sizeof(c->xxW);                   
    length+=sizeof(c->yW);                    
    length+=sizeof(c->yyW);                   
    length+=sizeof(c->xyW);                   
    length+=sizeof(c->sumWeight);             
    length+=sizeof(c->minContents);           
    length+=sizeof(c->maxContents);
    size_t headLength=length;
    length+=sizeof(c->contents[0])*c->contents.size();
    length+=sizeof(c->errors[0])*c->errors.size();
    hid_t memtype = H5Tcreate(H5T_COMPOUND,length);
    hid_t filetype = H5Tcreate (H5T_COMPOUND, length);
    size_t offset=0;
    {
      H5Tinsert (filetype, "nBinsX",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "nBinsX",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "nBinsY",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "nBinsY",offset, H5T_NATIVE_ULONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "minX",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "minX",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "maxX",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "maxX",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "minY",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "minY",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "maxY",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "maxY",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "binWidthX",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "binWidthX",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "binWidthY",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "binWidthY",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    hsize_t overUnderDims[]={3,3};
    hid_t overUnder_id= H5Tarray_create (H5T_NATIVE_UINT,2,overUnderDims);
    {
      H5Tinsert (filetype, "overUnderFlow",offset, overUnder_id);
      H5Tinsert (memtype,  "overUnderFlow",offset, overUnder_id);
      offset+=9*sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "overflow",offset, H5T_NATIVE_ULONG);
      H5Tinsert (memtype,  "overflow",offset, H5T_NATIVE_LONG);
      offset+=sizeof(size_t);
    }
    {
      H5Tinsert (filetype, "xW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "xW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "xxW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "xxW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "yW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "yW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "yyW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "yyW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "xyW",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "xyW",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "sumWeight",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "sumWeight",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "minContents",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "minContents",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    {
      H5Tinsert (filetype, "maxContents",offset, H5T_NATIVE_DOUBLE);
      H5Tinsert (memtype,  "maxContents",offset, H5T_NATIVE_DOUBLE);
      offset+=sizeof(double);
    }
    hsize_t arrayDims[]={c->contents.size()};
    hid_t arr_id= H5Tarray_create (H5T_NATIVE_DOUBLE,1,arrayDims);

    {
      H5Tinsert (filetype, "contents",offset, arr_id);
      H5Tinsert (memtype,  "contents",offset, arr_id);
      offset+=sizeof(c->contents[0])*c->contents.size();
    }
    {
      H5Tinsert (filetype, "errors",offset, arr_id);
      H5Tinsert (memtype,  "errors",offset, arr_id);
      offset+=sizeof(c->errors[0])*c->errors.size();
    }
    hsize_t     dims[]={1}; 
    hid_t space_id = H5Screate_simple(1, dims, NULL);
    hid_t dataset_id = H5Dcreate(_od, (prefix+c->name).c_str(), filetype, space_id,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    char *buff=new char[length];
    memcpy(buff, &c->nBinsX,headLength);
    memcpy(buff+headLength,&c->contents[0],sizeof(c->contents[0])*c->contents.size());
    memcpy(buff+headLength+sizeof(c->contents[0])*c->contents.size(),&c->errors[0],sizeof(c->errors[0])*c->errors.size());
    
    H5Dwrite (dataset_id, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buff);
    delete [] buff;
    H5Dclose(dataset_id);
    H5Sclose(space_id);
    H5Tclose(memtype);
    H5Tclose(filetype);
    H5Tclose(overUnder_id);
    H5Tclose(arr_id);
  }
  
  
}


void HDF5Driver::read(HistogramManager *mgr) const {
  
  
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  if (!_id) {
    for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
      if (mgr==(*m).second) {
	name = (*m).first;
	hid_t fileId = _ostrMap[name];
	_id = H5Gopen(fileId,"/",H5P_DEFAULT);
	break;
      }
    }
  }
  if (!_id) return;
  hsize_t nobj;
  H5Gget_num_objs(_id, &nobj);
  
  for (size_t i = 0; i < nobj; i++) {

    char memb_name[MAX_NAME];
    

    H5Gget_objname_by_idx(_id, (hsize_t)i,  memb_name, (size_t)MAX_NAME );
    hid_t otype =  H5Gget_objtype_by_idx(_id, (size_t)i );

    if (otype==H5G_DATASET) {
      hid_t dsid = H5Dopen(_id,memb_name,H5P_DEFAULT);

      std::string fullname=memb_name;
      std::string prefix=fullname.substr(0,2);
      std::string oname=fullname.substr(2,fullname.npos);
      if (prefix=="T:") {
	HDF5IOStore *store = new HDF5IOStore(dsid);
	mgr->newTable(oname,store);
      }
      else if (prefix=="2:") {
	Hist2D::Clockwork c;
	c.name       = std::string(oname);
	
	size_t length=0;
	length+=sizeof(c.nBinsX);
	length+=sizeof(c.nBinsY);
	length+=sizeof(c.minX);
	length+=sizeof(c.maxX);
	length+=sizeof(c.minY);
	length+=sizeof(c.maxY);
	length+=sizeof(c.binWidthX);              
	length+=sizeof(c.binWidthY);              
	length+=sizeof(c.overUnderFlow);              
	length+=sizeof(c.overflow);             
	length+=sizeof(c.xW);                    
	length+=sizeof(c.xxW);                   
	length+=sizeof(c.yW);                    
	length+=sizeof(c.yyW);                   
	length+=sizeof(c.xyW);                   
	length+=sizeof(c.sumWeight);             
	length+=sizeof(c.minContents);           
	length+=sizeof(c.maxContents);
	size_t headLength=length;
	hid_t memtype = H5Tcreate(H5T_COMPOUND,length);
	hid_t filetype = H5Tcreate (H5T_COMPOUND, length);
	size_t offset=0;
	{
	  H5Tinsert (filetype, "nBinsX",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "nBinsX",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "nBinsY",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "nBinsY",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "minX",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "minX",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "maxX",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "maxX",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "minY",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "minY",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "maxY",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "maxY",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "binWidthX",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "binWidthX",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "binWidthY",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "binWidthY",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	hsize_t overUnderDims[]={3,3};
	hid_t overUnder_id= H5Tarray_create (H5T_NATIVE_UINT,2,overUnderDims);
	{
	  H5Tinsert (filetype, "overUnderFlow",offset, overUnder_id);
	  H5Tinsert (memtype,  "overUnderFlow",offset, overUnder_id);
	  offset+=9*sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "overflow",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "overflow",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "xW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "xW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "xxW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "xxW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "yW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "yW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "yyW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "yyW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "xyW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "xyW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "sumWeight",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "sumWeight",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "minContents",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "minContents",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "maxContents",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "maxContents",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	
	
	// Read the data.

	H5Dread (dsid, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &c.nBinsX);

	c.contents.resize(c.nBinsX*c.nBinsY);
	c.contents.reserve(c.nBinsX*c.nBinsY);
	c.errors.resize(c.nBinsX*c.nBinsY);
	c.errors.reserve(c.nBinsX*c.nBinsY);


	hsize_t arrayDims[]={c.contents.size()};
	hid_t arr_id= H5Tarray_create (H5T_NATIVE_DOUBLE,1,arrayDims);

	length+=sizeof(c.contents[0])*c.contents.size();
	length+=sizeof(c.errors[0])*c.errors.size();
	H5Tset_size(memtype,length);
	H5Tset_size (filetype, length);


	{
	  H5Tinsert (filetype, "contents",offset, arr_id);
	  H5Tinsert (memtype,  "contents",offset, arr_id);
	  offset+=sizeof(c.contents[0])*c.contents.size();
	}
	{
	  H5Tinsert (filetype, "errors",offset, arr_id);
	  H5Tinsert (memtype,  "errors",offset, arr_id);
	  offset+=sizeof(c.errors[0])*c.errors.size();
	}
	hsize_t cLength=sizeof(c.contents[0])*c.contents.size();
	char *buff=new char[headLength+2*cLength];
	H5Dread (dsid, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buff);
	memcpy(&c.contents[0],buff+headLength,cLength);
	memcpy(&c.errors[0],buff+headLength+cLength,cLength);
	delete [] buff;
	
	Hist2D newHisto(&c);
	mgr->newHist2D(newHisto);

	H5Tclose(memtype);
	H5Tclose(filetype);
	H5Tclose(arr_id);
	H5Dclose(dsid);      
      }
      if (prefix=="1:") {
	Hist1D::Clockwork c;
	c.name       = std::string(oname);
	size_t length=0;
	length+=sizeof(c.nBins);
	length+=sizeof(c.min);
	length+=sizeof(c.max);
	length+=sizeof(c.binWidth);              
	length+=sizeof(c.overflow);              
	length+=sizeof(c.underflow);             
	length+=sizeof(c.entries);               
	length+=sizeof(c.xW);                    
	length+=sizeof(c.xxW);                   
	length+=sizeof(c.sumWeight);             
	length+=sizeof(c.minContents);           
	length+=sizeof(c.maxContents);
	size_t headLength=length;
	//length+=sizeof(c.contents[0])*c.contents.size();
	//length+=sizeof(c.errors[0])*c.errors.size();
	hid_t memtype = H5Tcreate(H5T_COMPOUND,length);
	hid_t filetype = H5Tcreate (H5T_COMPOUND, length);
	size_t offset=0;
	{
	  H5Tinsert (filetype, "nBins",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "nBins",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "min",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "min",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "max",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "max",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "binWidth",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "binWidth",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "overflow",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "overflow",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "underflow",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "underflow",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "entries",offset, H5T_NATIVE_ULONG);
	  H5Tinsert (memtype,  "entries",offset, H5T_NATIVE_ULONG);
	  offset+=sizeof(size_t);
	}
	{
	  H5Tinsert (filetype, "xW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "xW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "xxW",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "xxW",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "sumWeight",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "sumWeight",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "minContents",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "minContents",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	{
	  H5Tinsert (filetype, "maxContents",offset, H5T_NATIVE_DOUBLE);
	  H5Tinsert (memtype,  "maxContents",offset, H5T_NATIVE_DOUBLE);
	  offset+=sizeof(double);
	}
	
	// Read the data.

	H5Dread (dsid, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &c.nBins);

	c.contents.resize(c.nBins);
	c.contents.reserve(c.nBins);
	c.errors.resize(c.nBins);
	c.errors.reserve(c.nBins);


	hsize_t arrayDims[]={c.contents.size()};
	hid_t arr_id= H5Tarray_create (H5T_NATIVE_DOUBLE,1,arrayDims);

	length+=sizeof(c.contents[0])*c.contents.size();
	length+=sizeof(c.errors[0])*c.errors.size();
	H5Tset_size(memtype,length);
	H5Tset_size (filetype, length);


	{
	  H5Tinsert (filetype, "contents",offset, arr_id);
	  H5Tinsert (memtype,  "contents",offset, arr_id);
	  offset+=sizeof(c.contents[0])*c.contents.size();
	}
	{
	  H5Tinsert (filetype, "errors",offset, arr_id);
	  H5Tinsert (memtype,  "errors",offset, arr_id);
	  offset+=sizeof(c.errors[0])*c.errors.size();
	}
	hsize_t cLength=sizeof(c.contents[0])*c.contents.size();
	char *buff=new char[headLength+2*cLength];
	H5Dread (dsid, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, buff);
	memcpy(&c.contents[0],buff+headLength,cLength);
	memcpy(&c.errors[0],buff+headLength+cLength,cLength);
	delete [] buff;
	
	Hist1D newHisto(&c);
	mgr->newHist1D(newHisto);
	H5Tclose(memtype);
	H5Tclose(filetype);
	H5Tclose(arr_id);
	H5Dclose(dsid);      
      }
    }
    if (otype==H5G_GROUP) 
      {
	
	HistogramManager *newMgr=mgr->newDir(memb_name);
	hid_t oldId=_id;
	_id=H5Gopen(_id,memb_name,H5P_DEFAULT);
	read(newMgr);
	_id=oldId;
      }
    if (otype==H5G_TYPE) 
      {
	std::cout << "A datatype found" << std::endl;
      }
  }
  H5Gclose(_id);
}
	 
