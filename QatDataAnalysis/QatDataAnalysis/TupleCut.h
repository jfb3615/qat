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

#ifndef _TupleCut_h_
#define _TupleCut_h_
#include "QatDataAnalysis/Tuple.h"
#include "QatGenericFunctions/CutBase.h"
#include "QatGenericFunctions/AbsFunction.h"
//-----------------TupleCut-----------------------------//
// A lower limit, upper limit or window around some     //
// function of a tuple.  These can be anded, or'd or    //
// notted.                                              //
//------------------------------------------------------//

class TupleCut : public Cut<Tuple> {

 public:

  enum Type {GT,LT,GE,LE,NA};
  
  // Copy Constructor:
  TupleCut (const TupleCut & right);

  // Can say:  TupleCut cut(f,-1, 1);
  TupleCut (Genfun::GENFUNCTION f, double min, double max);

  // Can also say TupleCut cut (f, TupleCut::GT, 0),
  // Can also say TupleCut cut (f, TupleCut::LT, 0);
  TupleCut (Genfun::GENFUNCTION f, Type t, double val);

  // Don't forget you can and and or these, too.

  // Destructor:
  virtual ~TupleCut();

  // Clone 
  virtual TupleCut * clone (void) const;

  // Truth operator:
  virtual bool operator () (const Tuple & t) const;

 private:
  
  const Genfun::AbsFunction *_f;
  Type                       _t;
  double                    _v1;
  double                    _v2;
  
};

#endif
