#pragma once
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

// --------------------------------------------------------------------//
//                                                                     //
// A function of more than one variable uses this argument class to    //
// agglomerate the variables. It is similar to a vector.               // 
//                                                                     //
//---------------------------------------------------------------------//

#include <iostream>
#include <vector>
#include <iterator>
namespace Genfun {

  using Argument=std::vector<double>;


} // namespace Genfun

inline size_t dimension(const Genfun::Argument & a) {
  return a.size();
}

inline std::ostream & operator << (std::ostream & os, const Genfun::Argument & a) {
  std::ostream_iterator<double> oi(os,",");
  std::copy (a.begin(),a.end(),oi);
  return os;
}


