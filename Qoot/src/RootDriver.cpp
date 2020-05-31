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
#include "TTree.h"
#include "TFile.h"
#include "TList.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TKey.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unistd.h>
#include <memory>
#include <stdexcept>
//* -----------------declarations -------------------*//

class RootDriver: public IODriver {
 
public:

  // Constructor:
  RootDriver();

  // Destructor:
  ~RootDriver();

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
  mutable std::map<std::string, TFile            *> _ostrMap;
  mutable std::map<std::string, TFile            *> _istrMap;

  mutable TFile *_o;
  mutable TFile *_i;

};


// Creation routine:
extern "C" void *RootDriverCreate();






class RootIOStore: public TupleStore {

public:

  enum TCode {
    Double_c=0,
    Int_c,
    UInt_c,
    Float_c
  };
    
  // Constructor:
  RootIOStore(TTree * tree);

  // For input:
  virtual TupleConstLink operator [](size_t i) const;

  // Get the size:
  virtual size_t size() const {
    return (size_t) _tree->GetEntries();
  }

  AttributeListConstLink attributeList() const { 
    return internalTable->attributeList();
  }

private:

  // Destructor:
  virtual ~RootIOStore();

  

  TTree                         *_tree;
  
  Table                          *internalTable;
  TupleLink                      internalTuple;
  std::vector<std::string>       nList;
  std::vector<TLeaf *>           lList;
  size_t                         nAttributes;

  friend class ImaginaryFriend;

}; 


TupleConstLink RootIOStore::operator [] (size_t i) const {

  size_t limit = (size_t) (0.5+_tree->GetEntries());
  if (i>=limit) return NULL;

  _tree->GetEntry(i);
  internalTuple->uncache();

  return internalTuple;

}


RootIOStore::~RootIOStore() {
  delete internalTable;
  delete _tree;
}

RootIOStore::RootIOStore(TTree * tree):
_tree(tree),
internalTable(NULL),
internalTuple(NULL)
{
  TObjArray *bArray = tree->GetListOfLeaves();
  TObjArrayIter next(bArray);
  TObject *to;

  internalTable       = new Table("Internal Use Only");

  while ((to=next())) {
    
    TLeaf * leaf = dynamic_cast<TLeaf *> (to);
    if (leaf) {
      std::string typeName = leaf->GetTypeName();
      TBranch * branch = leaf->GetBranch();
      std::string branchName = branch ? std::string(branch->GetName()) : std::string("") ;
      std::string name     = branchName != leaf->GetName() ? branchName + std::string("_") + leaf->GetName() : leaf->GetName();
      if (typeName=="Float_t")  {
	internalTable->add(name,float(0));
      }
      else if (typeName=="Double_t") {
	internalTable->add(name,double(0));
      }
      else if (typeName=="Int_t")    {
	internalTable->add(name,int(0));
      }
      // Just added, not debugged:
      else if (typeName=="UChar_t")    {
	internalTable->add(name,size_t(0));
      }
      // End  addition
      else if (typeName=="UInt_t") {  
	internalTable->add(name,size_t(0));
      }
      else {
	std::cerr << "[RootDriver:  Unknown type " << typeName << " will be ignored on input]" << std::endl;
      }
      nList.push_back(name);
      lList.push_back(leaf);
    }
  }
  // Get the data:
  
  // End of header.  Now start the data:
  internalTuple       = (TupleLink) internalTable->capture();
    
  ValueList & vList = internalTuple->valueList();


  for (size_t j=0;j<nList.size();j++) {
    try {
      int i = internalTable->attribute(nList[j]).attrId();
      lList[j]->SetAddress((void *) vList[i].asCharStar() );
    }
    catch (std::runtime_error &) {
    }
  }
}

//*----------------implementation------------------*//


extern "C" void *RootDriverCreate() {
  return new RootDriver();
} 


RootDriver::RootDriver():_o(NULL),_i(NULL) {
  std::cerr << "[RootDriver:  HELLO.]" << std::endl;
}


RootDriver::~RootDriver() {
}

const HistogramManager *RootDriver::openManager(const std::string & filename)  const {
  // See if this file is indeed a root file:
  {
    std::ifstream ifile(filename.c_str());
    char buf[5]="    ";
    ifile >> buf[0] >> buf[1] >> buf[2] >> buf[3];
    if (std::string(buf)=="root") {
    }
    else {
      std::cerr << "[RootDriver: File NOT OK:" << filename << "]" << std::endl;
      return NULL;
    }
  }
  
  // See if you can open this file:
  TFile * stream = new TFile (filename.c_str(),"READ");
  if (!stream->IsOpen()) {
    delete stream;
    return NULL; 
  }
   
  _istrMap[filename]=stream;

   
  HistogramManager *mgr = new HistogramManager(filename);
  _mgrMap[filename]=mgr;
   
  read(mgr);
   
  std::cerr << "[RootDriver:  Open Histogram file " << filename << "]" << std::endl;
  
  return mgr;

}


HistogramManager *RootDriver::newManager(const std::string & filename)  const {

 // See if you can open this file:
  TFile *stream = new TFile(filename.c_str(),"recreate");
  if (!stream->IsOpen()) {
    delete stream;
    return NULL;
  }
                                                                                                           
  _ostrMap[filename]=stream;
                                                                                                           
  HistogramManager *mgr = new HistogramManager();
  _mgrMap[filename]=mgr;
                                                                                                           
  std::cerr << "[RootDriver:  New Histogram file " << filename << "]" << std::endl;

  return mgr;

}

// Writes the Histograms out.
void RootDriver::close (const HistogramManager *mgr) const {
  // Find the file name:
  std::map<std::string, HistogramManager *>::iterator 
    begin=_mgrMap.begin(),
    end=_mgrMap.end(),m=begin;
  std::string filename;
  while (m!=end) {
    m++;
    if ((*m).second==mgr) {
      filename = (*m).first;
      break;
    }
  }
  
  if (filename!="") {
    {
      TFile *file = _ostrMap[filename];
      if (file) file->Close();
    }
    {
      TFile *file = _istrMap[filename];
      if (file) file->Close();
    }
    {
      std::map<std::string, HistogramManager *>::iterator d=_mgrMap.find(filename);
      if (d!=_mgrMap.end()) _mgrMap.erase(d);
    }
    {
      std::map<std::string, TFile *>::iterator d=_istrMap.find(filename);
      if (d!=_istrMap.end()) _istrMap.erase(d);
    }
    {
      std::map<std::string, TFile *>::iterator d=_ostrMap.find(filename);
      if (d!=_ostrMap.end()) _ostrMap.erase(d);
    }
  }
  
  
}

// Closes the file:
void RootDriver::write (HistogramManager *mgr) const {

  // Find the output file
  // Find the output stream for this:
  bool topLevel(false);
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
    if (mgr==(*m).second) {
      name = (*m).first;
      _o = _ostrMap[name];
      _o->cd();
      topLevel=true;
      break;
    }
  }
   
   
  // Write out all of the ntuples:

  HistogramManager::DConstIterator d;
  for (d=mgr->beginDir();d!=mgr->endDir();d++) {
    TDirectory *dir=gDirectory, *newDir=gDirectory->mkdir((*d)->name().c_str());
    newDir->cd();
    write (*d);
    dir->cd();
  }

  // Write out all ntuples:
  HistogramManager::TConstIterator t;
  for (t=mgr->beginTable();t!=mgr->endTable();t++) {
    
    float         *fBuff = new float        [(**t).numAttributes()];
    double        *dBuff = new double       [(**t).numAttributes()];
    int           *iBuff = new int          [(**t).numAttributes()];
    unsigned int  *uBuff = new unsigned int [(**t).numAttributes()];

    //    float     fBuff[(**t).numAttributes()];
    //    double    dBuff[(**t).numAttributes()];
    //    int       iBuff[(**t).numAttributes()];
    //    size_t    uBuff[(**t).numAttributes()];

    TTree *tree = new TTree((**t).name().c_str(), (**t).name().c_str());
    for (unsigned int i=0;i<(**t).numAttributes();i++) {
      std::string name     = (**t).attribute(i).name();
      {
	std::string typeName = (**t).attribute(i).typeName();
	
	if (typeName=="d") {
	  tree->Branch(name.c_str(), dBuff+i, (name+"/D").c_str());
	}
	
	else if (typeName=="f") {
	  tree->Branch(name.c_str(), fBuff+i, (name+"/F").c_str());
	}
	
	else if (typeName=="i") {
	  tree->Branch(name.c_str(), iBuff+i, (name+"/I").c_str());
	}
	
	else if (typeName=="j" || typeName=="m" ) {
	  tree->Branch(name.c_str(), uBuff+i, (name+"/i").c_str());
	}
	else {
	  std::ostringstream msg;
	  msg << "RootDriver: cannot write data in of RTTI type " << typeName; 
	  throw std::runtime_error(msg.str().c_str());
	}
      }
    }    


    for (size_t n = 0; ; n++) {

      TupleConstLink thisTuple = (**t)[n];
      if (!thisTuple) break;

      for (unsigned int j=0;j<(*t)->numAttributes();j++) {
	{
	  if (thisTuple->read(iBuff[j],j)) continue;
	  else if (thisTuple->read(dBuff[j],j)) continue;
	  else if (thisTuple->read(uBuff[j],j)) continue;
	  else if (thisTuple->read(fBuff[j],j)) continue;
          throw std::runtime_error("Unknown data type cannot be written");
        }
      }
      tree->Fill();  // Once per event?
    }
    delete [] fBuff;
    delete [] dBuff;
    delete [] iBuff;
    delete [] uBuff;

  }

  for (HistogramManager::H1ConstIterator h=mgr->beginH1();h!=mgr->endH1();h++) {
    TH1D  *hist = new TH1D((**h).name().c_str(),(**h).name().c_str(), (**h).nBins(), (**h).min(), (**h).max());
    for (size_t c=0;c<(**h).nBins();c++) {
      hist->SetBinContent(c+1,(**h).bin(c));
      hist->SetBinError(c+1,(**h).binError(c));
    }
    hist->SetBinContent(0,(**h).underflow());
    hist->SetBinContent((**h).nBins()+1, (**h).overflow());
    hist->SetEntries((**h).entries());

  }

  for (HistogramManager::H2ConstIterator h=mgr->beginH2();h!=mgr->endH2();h++) {
    TH2D  *hist = new TH2D((**h).name().c_str(),(**h).name().c_str(), 
			   (**h).nBinsX(), (**h).minX(), (**h).maxX(),
			   (**h).nBinsY(), (**h).minY(), (**h).maxY());
    for (size_t c=0;c<(**h).nBinsX();c++) {
      for (size_t d=0; d<(**h).nBinsY();d++) {
	hist->SetBinContent(c+1,d+1,(**h).bin(c,d));
	hist->SetBinError(c+1,d+1, (**h).binError(c,d));
      }
    }
    hist->SetBinContent(0,                 0,                 (**h).overflow(Hist2D::Underflow,Hist2D::Underflow));
    hist->SetBinContent(0,                 (**h).nBinsY()+1,  (**h).overflow(Hist2D::Underflow,Hist2D::Overflow));
    hist->SetBinContent((**h).nBinsX()+1,  (**h).nBinsY()+1,  (**h).overflow(Hist2D::Overflow, Hist2D::Overflow));
    hist->SetBinContent((**h).nBinsX()+1,  0,                 (**h).overflow(Hist2D::Overflow, Hist2D::Underflow));

    hist->SetBinContent(1,                 0,                 (**h).overflow(Hist2D::InRange,  Hist2D::Underflow));
    hist->SetBinContent(1,                 (**h).nBinsY()+1,  (**h).overflow(Hist2D::InRange,  Hist2D::Overflow));
    hist->SetBinContent((**h).nBinsX()+1,  1,                 (**h).overflow(Hist2D::Overflow, Hist2D::InRange));
    hist->SetBinContent(0,                 1,                 (**h).overflow(Hist2D::Underflow,Hist2D::InRange));
    
    hist->SetEntries((**h).entries());
    
  }
  
  if (topLevel) _o->Write();       // Once per file?

}


void RootDriver::read(HistogramManager *mgr) const {
  // Find the output stream for this:
  std::string name;
  std::map<std::string , HistogramManager *>::iterator m;
  for (m=_mgrMap.begin();m!=_mgrMap.end();m++) {
    if (mgr==(*m).second) {
      name = (*m).first;
      _i = _istrMap[name];
      _i->cd();
      break;
    }
  }

  std::set<std::string> nameSet;

  TList *list = gDirectory->GetListOfKeys();
  TListIter next(list);
  TObject *to;
  while ((to=next())) {

    TKey *key = dynamic_cast<TKey *> (to);
    if (key) {
      TObject *o = gROOT->FindObject(key->GetName());
      if (o) {
	{
	  TTree * tree = dynamic_cast<TTree *> (o);
	  if (tree) {
	    std::string treeName=key->GetName();
	    if (nameSet.find(treeName)==nameSet.end()) {
	      if (tree->GetEntries()) {
		RootIOStore *store = new RootIOStore(tree);
		nameSet.insert(treeName);
		/*Table * t = */ mgr->newTable(treeName,store);
	      }
	    }
	    else {
	    }
	    continue;
	  }
	}
	{
	  TH2 *h = dynamic_cast<TH2 *> (o);
	  if (h) {
	    Hist2D::Clockwork c;
	    c.name       = std::string(key->GetName());
	    c.nBinsX      = h->GetNbinsX();                            
	    c.nBinsY      = h->GetNbinsY();                            
	    c.binWidthX   = h->GetXaxis()->GetBinWidth(0);                         
	    c.binWidthY   = h->GetYaxis()->GetBinWidth(0);                         
	    c.minX         = h->GetXaxis()->GetBinLowEdge(1);                       
	    c.maxX         = c.minX + c.nBinsX*c.binWidthX;                
	    c.minY         = h->GetYaxis()->GetBinLowEdge(1);
	    c.maxY         = c.minY + c.nBinsY*c.binWidthY;                

	    c.overUnderFlow[Hist2D::Underflow][Hist2D::Underflow]   = (int) (0.5+ h->GetBinContent(0,0));                
	    c.overUnderFlow[Hist2D::Overflow] [Hist2D::Underflow]   = (int) (0.5+ h->GetBinContent(c.nBinsX+1,0));                
	    c.overUnderFlow[Hist2D::Overflow] [Hist2D::Overflow]    = (int) (0.5+ h->GetBinContent(c.nBinsX+1,c.nBinsY+1));                
	    c.overUnderFlow[Hist2D::Underflow][Hist2D::Overflow]    = (int) (0.5+ h->GetBinContent(0,c.nBinsY+1));                


	    for (unsigned int i=1;i<c.nBinsX+1; i++) c.overUnderFlow[Hist2D::InRange] [Hist2D::Underflow]   += (int) (0.5+h->GetBinContent(i,0));                
	    for (unsigned int i=1;i<c.nBinsY+1; i++) c.overUnderFlow[Hist2D::Overflow] [Hist2D::InRange]    += (int) (0.5+h->GetBinContent(c.nBinsX+1,i));                
	    for (unsigned int i=1;i<c.nBinsX+1; i++) c.overUnderFlow[Hist2D::InRange] [Hist2D::Overflow]    += (int) (0.5+h->GetBinContent(i,c.nBinsY+1));                
	    for (unsigned int i=1;i<c.nBinsY+1; i++) c.overUnderFlow[Hist2D::Underflow][Hist2D::InRange]    += (int) (0.5+h->GetBinContent(0,i));                
	   
	    c.overUnderFlow[Hist2D::InRange][Hist2D::InRange]= (int) (0.5+h->GetEntries());


	    c.overflow    = 
	      c.overUnderFlow[0][0] +                                                                                     
	      c.overUnderFlow[0][1] +                                                                                     
	      c.overUnderFlow[0][2] +                                                                                     
	      c.overUnderFlow[1][0] +                                                                                     
	      c.overUnderFlow[1][2] +                                                                                     
	      c.overUnderFlow[2][0] +                                                                                     
	      c.overUnderFlow[2][1] +                                                                                     
	      c.overUnderFlow[2][2];                                                                                     

	    c.xW          = h->GetSumOfWeights()*h->GetMean(1);                      
	    c.yW          = h->GetSumOfWeights()*h->GetMean(2);                      

	    c.xxW         = h->GetSumOfWeights()*(h->GetRMS(1)*h->GetRMS(1) + h->GetMean(1)*h->GetMean(1));                            
	    c.yyW         = h->GetSumOfWeights()*(h->GetRMS(2)*h->GetRMS(2) + h->GetMean(2)*h->GetMean(2));                            
	    c.xyW         = h->GetSumOfWeights()*(h->GetCovariance(1,2) + h->GetMean(1)*h->GetMean(2));                            


	    c.sumWeight   = h->GetSumOfWeights();                      
	    c.minContents = h->GetMinimum();                           
	    c.maxContents = h->GetMaximum();                           
	    
	    c.contents.resize(c.nBinsX*c.nBinsY);
	    c.contents.reserve(c.nBinsX*c.nBinsY);
	    c.errors.resize(c.nBinsX*c.nBinsY);
	    c.errors.reserve(c.nBinsX*c.nBinsY);

	    for (unsigned int i=0;i<c.nBinsX;i++) {
	      for (unsigned int j=0;j<c.nBinsY; j++) {
		size_t ii = c.nBinsX*j+i;
		c.contents[ii]=(h->GetBinContent(i+1,j+1));
		c.errors[ii]  =(h->GetBinError(i+1,j+1)*h->GetBinError(i+1,j+1));
	      }
	    }
	    Hist2D newHisto(&c);
	    mgr->newHist2D(newHisto);
	    continue;
	  }
	}
	{
	  TH1 *h = dynamic_cast<TH1 *> (o);
	  if (h) {
	    Hist1D::Clockwork c;
	    c.name       = std::string(key->GetName());
	    c.binWidth    = h->GetBinWidth(0);                         
	    c.nBins       = h->GetNbinsX();                            
	    c.min         = h->GetBinLowEdge(1);                       
	    c.max         = c.min + c.nBins*c.binWidth;                
	    c.underflow   = (int) (0.5+h->GetBinContent(0));                
	    c.overflow    = (int) (0.5+h->GetBinContent(c.nBins+1));         
	    c.entries     = (int) (0.5+h->GetEntries());                     
	    c.xW          = h->GetSumOfWeights()*h->GetMean();                      
	    c.xxW         = h->GetSumOfWeights()*(h->GetRMS()*h->GetRMS() + h->GetMean()*h->GetMean());                            
	    c.sumWeight   = h->GetSumOfWeights();                      
	    c.minContents = h->GetMinimum();                           
	    c.maxContents = h->GetMaximum();                           
	    
	    for (unsigned int i=0;i<c.nBins;i++) {
	      c.contents.push_back(h->GetBinContent(i+1));
	      c.errors.push_back(h->GetBinError(i+1)*h->GetBinError(i+1));
	    }
	    
	    Hist1D newHisto(&c);
	    mgr->newHist1D(newHisto);
	    continue;
	  }
	}

	{
	  TDirectory *d = dynamic_cast<TDirectory *> (o);
	  if (d) {
	    TDirectory *oldDirectory =  gDirectory;
	    HistogramManager *newMgr=mgr->newDir(d->GetName());
	    d->cd();
	    read(newMgr);
	    oldDirectory->cd();
	    continue;
	  }
	}
	{
	  std::cerr << "[RootDriver:  Unknown object " << o->GetName() <<"(" <<  o->ClassName() << ")]" << std::endl;

	}
      }
    }
  }
}
