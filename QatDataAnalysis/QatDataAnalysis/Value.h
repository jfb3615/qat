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

#ifndef _Value_h_
#define _Value_h_
#include <vector>
#include <algorithm>
#include <iostream>

class Value {

 public:

  Value(const void * value,  size_t sizeInBytes):
    _rep(std::vector<char>(sizeInBytes)){
      std::copy ((char *) value, (char *) value + sizeInBytes, _rep.begin());
    }

  const char * asCharStar() const { return & _rep[0];}
  
  void clear() {
    std::fill(_rep.begin(),_rep.end(),0);
  }
  
  template <typename T> void setValue (const T & t) {
    * ((T *) & _rep[0]) = t;
  }

 private:

  std::vector<char> _rep;
};


#include "QatDataAnalysis/Value.icc"
#endif
