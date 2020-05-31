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

#include "QatDataAnalysis/Hist1DMaker.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatDataAnalysis/Attribute.h"
#include "QatDataAnalysis/Table.h"
#include "QatGenericFunctions/Argument.h"
class Hist1DMaker::Clockwork {

public:

  Genfun::AbsFunction   *f;
  size_t                nBins;
  double                min;
  double                max;
  Genfun::AbsFunction   *w;
  
};


Hist1DMaker::Hist1DMaker (Genfun::GENFUNCTION f, size_t nbinsx, double min, double max,
			  const Genfun::AbsFunction *weight) : c(new Clockwork()) {
  c->f         = f.clone();
  c->nBins     = nbinsx;
  c->min       = min;
  c->max       = max;
  c->w         = weight ? weight->clone() : NULL;
}

Hist1DMaker::~Hist1DMaker() {
  delete c->f;
  delete c->w;
  delete c;
}


Hist1D Hist1DMaker::operator * (const Table & table) const {

  Hist1D h(table.name(),c->nBins, c->min, c->max);

  
  for  (size_t t=0; ; t++) {
    
    TupleConstLink tuple = table[t];
    if (!tuple) break;

    const Genfun::Argument & a  = tuple->asDoublePrec();
    
    if (c->w) {
      Genfun::GENFUNCTION W=*c->w;
      h.accumulate((*(c->f))(a), W(a));
    }
    else {
      h.accumulate((*(c->f))(a));
    }    
  }
  return h;
}
