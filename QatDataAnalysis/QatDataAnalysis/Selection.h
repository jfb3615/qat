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

#ifndef _Selection_h_
#define _Selection_h_
#include "QatDataAnalysis/Table.h"
#include "QatDataAnalysis/Tuple.h"
#include "QatGenericFunctions/CutBase.h"
// -----------------------------------------------------//
// This class defines selection operations on tables:   //
// -----------------------------------------------------//

class Selection {
 
 public:

  // Constructor:
  Selection( const Cut<Tuple> & cut);

  // Copy Constructor:
  Selection (const Selection & sel);

  // Destructor:
  ~Selection();

  // Assignment
  Selection & operator = (const Selection & sel);

  // Operation on Table
  Table operator * (const Table & table) const;


 private:
  
  const Cut<Tuple> * _cut;
  
};

#endif
