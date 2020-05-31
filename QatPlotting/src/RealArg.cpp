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



#include "QatPlotting/RealArg.h"
namespace RealArg {

  Eq::Eq(double value): _value(value){}
  bool Eq::operator() (const double & x) const {return x==_value;}
  Eq *Eq::clone() const { return new Eq(*this);}

  Lt::Lt(double value): _value(value){}
  bool Lt::operator() (const double & x) const {return x<_value;}
  Lt *Lt::clone() const { return new Lt(*this);}

  Gt::Gt(double value): _value(value) {}
  bool Gt::operator() (const double & x) const {return x>_value;}
  Gt *Gt::clone() const { return new Gt(*this);}

}

