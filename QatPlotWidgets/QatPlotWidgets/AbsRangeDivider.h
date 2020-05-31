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

//---------------------------------------------------------------------------//
//                                                                           //
// AbsRangeDivider.  This class takes the extremites of a range of values    //
// and figures out how to subdivide it, providing also information about     //
// the range                                                                 //
//                                                                           //
//                                                                           //
//---------------------------------------------------------------------------//
#ifndef _AbsRangeDivider_h_
#define _AbsRangeDivider_h_
#include "QatPlotWidgets/RangeDivision.h"
class AbsRangeDivider {

public:


  // Default constructor;
  AbsRangeDivider();
  
  // Destructor:
  virtual ~AbsRangeDivider();

  // Set the range:
  virtual void setRange(double min, double max)=0;
  virtual void setMin(double min)=0;
  virtual void setMax(double max)=0;


  // Get the number of subdivisions:
  virtual int getNumSubdivisions() const=0;

  // Get the location of each subdivision:
  virtual const RangeDivision & getSubdivision(int i) const= 0;

  // Get the validity of each subdivision:
  virtual bool isValid(int i) const =0;

};
#endif
