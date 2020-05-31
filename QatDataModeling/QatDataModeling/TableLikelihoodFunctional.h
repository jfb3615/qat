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
// This functional returns the Likelihood of a functoin        //
// given some data                                             //
//                                                             //
//-------------------------------------------------------------//
#ifndef _TableLikelihoodFunctional_h_
#define _TableLikelihoodFunctional_h_
#include "QatGenericFunctions/AbsFunctional.h"
#include "QatGenericFunctions/ArgumentList.h"
class Table;
class TableLikelihoodFunctional:public Genfun::AbsFunctional {
  
 public:
  
  // Constructor:
  TableLikelihoodFunctional(const Table & table);
  
  // Destructor:
  ~TableLikelihoodFunctional();
  
  // Evaluate ChiSquared of a function w.r.t the data.
  virtual double operator () (const Genfun::AbsFunction & function) const;
  
 private:
  
  const Table &_table;
  
};

#endif
