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

//-------------------------------------------------------------//
//                                                             //
// This functional returns the ChiSquared of a function        //
// with respect to some histogram.                             //
//                                                             //
//-------------------------------------------------------------//

#ifndef _HistChi2Functional_h_
#define _HistChi2Functional_h_
#include "QatGenericFunctions/AbsFunctional.h"
class Hist1D;
class HistChi2Functional:public Genfun::AbsFunctional {

public:
  
  // Constructor:
  HistChi2Functional(const Hist1D * histogram, 
		     double lower=-1E100, 
		     double upper= 1E100, 
		     bool integrate=false);

  // Destructor:
  ~HistChi2Functional();

  // Copy constructor
  HistChi2Functional(const HistChi2Functional &);

  // Evaluate ChiSquared of a function w.r.t the histogram
  virtual double operator () (const Genfun::AbsFunction & function) const;

private:
  
  const Hist1D       *_histogram;  // does not own this histogram.
  double              _minValue;   // min bin value for fit.
  double              _maxValue;   // max bin value for fit.
  bool                _integrate;  // integration flag.

  // These are caches for Histogram-like quantities. They are 
  // held here because they are expensive to extract from the
  // histogram:

  int   _nBins;
  float _min;
  float _max;
  float _delta;
  float _width;
  
  
  // Assignment is illegal:
  const HistChi2Functional & operator= (const HistChi2Functional &);


};
#endif


