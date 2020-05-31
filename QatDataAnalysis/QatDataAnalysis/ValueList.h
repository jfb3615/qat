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

#ifndef  ValueList_h_
#define  ValueList_h_
#include "QatDataAnalysis/Value.h"
#include <vector>

class ValueList {

 public:

  // Type Definitions:
  typedef std::vector<Value>::const_iterator ConstIterator;

  // Constructor:
  inline ValueList();

  // Destructor:
  inline ~ValueList();

  // Add an attribute to the list:
  inline void add(const Value & value);

  // Iterate over the attributes;
  inline ConstIterator begin () const;
  inline ConstIterator end() const;

  // Random access:
  inline Value & operator[] (size_t i) ;
  inline const Value & operator[] (size_t i) const;

  // Size of the attribute list;
  inline size_t size() const;
  
  inline void clear();

 private:

  std::vector<Value> _valueList;

};

#include "QatDataAnalysis/ValueList.icc"
#endif
