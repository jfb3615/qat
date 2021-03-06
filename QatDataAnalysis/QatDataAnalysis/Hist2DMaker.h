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

#ifndef _HIST2DMAKER_H_
#define _HIST2DMAKER_H_
//-------------------------------------------------------------------//
// Class which allows to make a histogram from an ntuple.            //
//-------------------------------------------------------------------//
#include <string>
#include "QatGenericFunctions/AbsFunction.h"
class Table;
class Hist2D;

class Hist2DMaker {

 public:


  // Constructor;
  Hist2DMaker(Genfun::GENFUNCTION fx, size_t nbinsX, double minX, double max,
	      Genfun::GENFUNCTION fy, size_t nbinsY, double minY, double maxY,
	      const Genfun::AbsFunction *weight=NULL);

  // Destructor:
  ~Hist2DMaker();

  // Action:
  Hist2D operator * (const Table & t) const;

 private:

  Hist2DMaker & operator = (const Hist2DMaker & );
  Hist2DMaker(const Hist2DMaker &);

  class Clockwork;
  Clockwork *c;

};

#endif
