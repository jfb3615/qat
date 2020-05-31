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

// ----------------------------------------------------------//
//                                                           //
// Set of domain restrictions for 1-D functions:             //
//                                                           //
//                                                           //
// RealArg::Eq(value)                                        //
// RealArg::Gt(value)                                        //
// RealArg::Lt(value)                                        //
//                                                           //
// These are useful for specifying where a function is valid //
// i.e. the domain of the function.                          //
//                                                           //
// ----------------------------------------------------------//

#ifndef _REALARG_HH_
#define _REALARG_HH_
#include "QatGenericFunctions/CutBase.h"
namespace RealArg {
  
  class Eq:         public Cut<double> {
  public:
    Eq(double value);
    virtual bool operator() (const double & x) const;
    virtual Eq *clone() const;
  private:
    double _value;  // value;
  };
  
  class Gt:         public Cut<double> {
  public:
    Gt(double value);
    virtual bool operator() (const double & x) const;
    virtual Gt *clone() const;
  private:
    double _value;  // value;
  };
  
  class Lt:         public Cut<double> {
  public:
    Lt(double value);
    virtual bool operator() (const double & x) const;
    virtual Lt *clone() const;
  private:
    double _value;  // value;
  };
  
}

#endif
