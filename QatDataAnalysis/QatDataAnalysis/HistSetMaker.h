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

#ifndef _HISTSETMAKER_H_
#define _HISTSETMAKER_H_
//-------------------------------------------------------------------//
// Class which allows to make a histogram from an ntuple.            //
//-------------------------------------------------------------------//
#include <string>
#include "QatGenericFunctions/AbsFunction.h"
class Table;
class HistogramManager;

class HistSetMaker {

 public:

  // Constructor;
  HistSetMaker();

  // Destructor:
  ~HistSetMaker();

  // Action:
  void exec(const Table & t, HistogramManager *manager) const;

  // Schedule 1 D Histogram
  void scheduleHist1D(const std::string & name, Genfun::GENFUNCTION f, unsigned int nbins, double min, double max,
		      const Genfun::AbsFunction *weight=NULL);
  
  
  // Schedule 2 D Histogram
  void scheduleHist2D(const std::string & name, 
		      Genfun::GENFUNCTION fX, unsigned int nbinsX, double minX, double maxX,
		      Genfun::GENFUNCTION fY, unsigned int nbinsY, double minY, double maxY,
		      const Genfun::AbsFunction *weight=NULL);

  // Access stored info:

  // Number of Histograms:
  unsigned int numH1D() const;
  unsigned int numH2D() const;
  
  // Name of Histograms
  const std::string & nameH1D(unsigned int i) const;
  const std::string & nameH2D(unsigned int i) const;

  // ...name may then be used to retrieve histogram from manager and thus find
  // additional information on the histograms that are created.  
  
 private:

  HistSetMaker & operator = (const HistSetMaker & );
  HistSetMaker(const HistSetMaker &);

  class Clockwork;
  Clockwork *c;

};

#endif
