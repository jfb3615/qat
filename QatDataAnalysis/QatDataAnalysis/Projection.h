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

#ifndef _Projection_h_
#define _Projection_h_
#include "QatDataAnalysis/Tuple.h"
#include "QatDataAnalysis/Table.h"
#include "QatGenericFunctions/CutBase.h"
// -----------------------------------------------------//
// This class defines projection operations on tables:  //
// -----------------------------------------------------//

class Projection {
 
 public:

  // Constructor:
  Projection();

  // Copy Constructor:
  Projection (const Projection & P);

  // Destructor:
  ~Projection();

  // Assignment
  Projection & operator = (const Projection & sel);

  // Operation on Table
  Table operator * (const Table & table) const;

  // Add a datum
  void add(const std::string & name);
  

 private:
  
  std::set<std::string> _nameList;
  
};

#endif
