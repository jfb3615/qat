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

#ifndef  AttributeList_h_
#define  AttributeList_h_
#include "QatDataAnalysis/Attribute.h"
#include "QatDataAnalysis/RCBase.h"
#include <vector>

class AttributeList: public Genfun::RCBase {

 public:

  // Type Definitions:
  typedef std::vector<Attribute>::const_iterator ConstIterator;

  // Constructor:
  AttributeList();

  // Add an attribute to the list:
  void add( const std::string & name, const std::type_info & type);

  // Lock the attribute list:
  void lock();

  // Iterate over the attributes;
  inline ConstIterator begin () const;
  inline ConstIterator end() const;

  // Random access:
  inline Attribute & operator[] (size_t i) ;
  inline const Attribute & operator[] (size_t i) const;

  // Size of the attribute list;
  inline size_t size() const;
  

 private:

  // Destructor:
  ~AttributeList();

  std::vector<Attribute> _attrList;
  bool _locked;

};

#include "QatDataAnalysis/AttributeList.icc"
#endif
