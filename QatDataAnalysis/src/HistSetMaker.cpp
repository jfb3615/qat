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

#include "QatDataAnalysis/HistSetMaker.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatDataAnalysis/Hist2D.h"
#include "QatDataAnalysis/HistogramManager.h"
#include "QatDataAnalysis/Attribute.h"
#include "QatDataAnalysis/Table.h"
#include "QatGenericFunctions/Argument.h"
class HistSetMaker::Clockwork {

public:

  struct Hist1DDescriptor {
    Hist1DDescriptor() :f(NULL),hist(NULL),weight(NULL) {}
    std::string           name;
    Genfun::AbsFunction   *f;
    unsigned int          nBins;
    double                min;
    double                max;
    Hist1D                *hist;
    const Genfun::AbsFunction   *weight;
  };


  struct Hist2DDescriptor {
    Hist2DDescriptor() :fX(NULL),fY(NULL),hist(NULL),weight(NULL) {}
    std::string           name;
    Genfun::AbsFunction   *fX;
    unsigned int          nBinsX;
    double                minX;
    double                maxX;
    Genfun::AbsFunction   *fY;
    unsigned int          nBinsY;
    double                minY;
    double                maxY;
    Hist2D                *hist;
    const Genfun::AbsFunction   *weight;
  };

  std::vector<Hist1DDescriptor>  hist1DList;
  std::vector<Hist2DDescriptor>  hist2DList;
};

HistSetMaker::HistSetMaker():c(new Clockwork()){
}

void HistSetMaker::scheduleHist1D(const std::string & name, Genfun::GENFUNCTION f, unsigned int nbins, double min, double max, const Genfun::AbsFunction *w) {
  Clockwork::Hist1DDescriptor descriptor;
  c->hist1DList.push_back(descriptor);
  c->hist1DList.back().name=name;
  c->hist1DList.back().f= f.clone();
  c->hist1DList.back().nBins= nbins;
  c->hist1DList.back().min  = min;
  c->hist1DList.back().max  = max;
  c->hist1DList.back().weight = w ? w->clone(): NULL;
}

void HistSetMaker::scheduleHist2D(const std::string & name, 
				  Genfun::GENFUNCTION fX, unsigned int nbinsX, double minX, double maxX,
				  Genfun::GENFUNCTION fY, unsigned int nbinsY, double minY, double maxY,
				  const Genfun::AbsFunction *w) {
  Clockwork::Hist2DDescriptor descriptor;
  c->hist2DList.push_back(descriptor);
  c->hist2DList.back().name=name;
  c->hist2DList.back().fX= fX.clone();
  c->hist2DList.back().nBinsX= nbinsX;
  c->hist2DList.back().minX  = minX;
  c->hist2DList.back().maxX  = maxX;
  c->hist2DList.back().fY= fY.clone();
  c->hist2DList.back().nBinsY= nbinsY;
  c->hist2DList.back().minY  = minY;
  c->hist2DList.back().maxY  = maxY;
  c->hist2DList.back().weight = w ? w->clone(): NULL;
}

HistSetMaker::~HistSetMaker() {
  for (unsigned int i=0;i<c->hist1DList.size();i++) {
    delete c->hist1DList[i].f;
    delete c->hist1DList[i].weight;
  }
  for (unsigned int i=0;i<c->hist2DList.size();i++) {
    delete c->hist2DList[i].fX;
    delete c->hist2DList[i].fY;
    delete c->hist2DList[i].weight;
  }
  delete c;
}


void HistSetMaker::exec (const Table & table, HistogramManager *m) const {

  // Make some histograms;
  for (unsigned int i=0;i<c->hist1DList.size();i++) {
    c->hist1DList[i].hist = m->newHist1D(c->hist1DList[i].name,
					 c->hist1DList[i].nBins,
					 c->hist1DList[i].min,
					 c->hist1DList[i].max);
  }
  for (unsigned int i=0;i<c->hist2DList.size();i++) {
    c->hist2DList[i].hist = m->newHist2D(c->hist2DList[i].name,
					 c->hist2DList[i].nBinsX,
					 c->hist2DList[i].minX,
					 c->hist2DList[i].maxX,
					 c->hist2DList[i].nBinsY,
					 c->hist2DList[i].minY,
					 c->hist2DList[i].maxY);
  }
  
  // Do not worry about cleaning up that memory.  Histograms are
  // owned by the manager!
  for  (unsigned int t=0; ; t++) {
    
    TupleConstLink tuple = table[t];
    if (!tuple) break;

    const Genfun::Argument & a  = tuple->asDoublePrec();
    
    for (unsigned int i=0;i<c->hist1DList.size();i++) {
      if (c->hist1DList[i].weight) {
	Genfun::GENFUNCTION W=*c->hist1DList[i].weight;
	c->hist1DList[i].hist->accumulate((*(c->hist1DList[i].f))(a),W(a));
      }
      else {
	c->hist1DList[i].hist->accumulate((*(c->hist1DList[i].f))(a));
      }
    }
    
    for (unsigned int i=0;i<c->hist2DList.size();i++) {
      if (c->hist2DList[i].weight) {
	Genfun::GENFUNCTION W=*c->hist2DList[i].weight;
	c->hist2DList[i].hist->accumulate((*(c->hist2DList[i].fX))(a), (*(c->hist2DList[i].fY))(a) ,W(a) );
      }
      else {
	c->hist2DList[i].hist->accumulate((*(c->hist2DList[i].fX))(a), (*(c->hist2DList[i].fY))(a)  );
      }

    }
    
  }
  

}


// Number of Histograms:
unsigned int HistSetMaker::numH1D() const {
  return c->hist1DList.size();
}
unsigned int HistSetMaker::numH2D() const{
  return c->hist2DList.size();
}

// Name of Histograms
const std::string & HistSetMaker::nameH1D(unsigned int i) const {
  return c->hist1DList[i].name;
}
const std::string & HistSetMaker::nameH2D(unsigned int i) const {
  return c->hist2DList[i].name;
}
