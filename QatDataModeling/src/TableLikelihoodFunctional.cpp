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

#include "QatDataModeling/TableLikelihoodFunctional.h"
#include "QatDataAnalysis/Table.h"
#include "QatDataAnalysis/Tuple.h"
#include "QatGenericFunctions/Argument.h"
#include "QatGenericFunctions/AbsFunction.h"
#include <cmath>
TableLikelihoodFunctional::TableLikelihoodFunctional(const Table & table):
_table(table)
{}

TableLikelihoodFunctional::~TableLikelihoodFunctional() {
}

double TableLikelihoodFunctional::operator () (const Genfun::AbsFunction & function) const {
  double logLikelihood(0);

 for  (size_t t=0; ; t++) {

   TupleConstLink tuple = _table [t];
   if (!tuple) break;
	    
   const Genfun::Argument & a  = tuple->asDoublePrec();

    double f = function(a);
    if (f<0)
      std::cerr
	<< "Warning.. negative likelihood arg[" << t << "]=" << a 
	<< std::endl;
    logLikelihood -= log(f);

 }
  return 2.0*logLikelihood;
}

