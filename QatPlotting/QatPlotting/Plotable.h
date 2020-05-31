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



//--------------------------------------------------------------------------//
// Class Plotable                                                           //
//                                                                          //
// Joe Boudreau June 2K                                                     //
//                                                                          //
// This class is a base class for all plotable objects.  It describes       //
// the plottable thing in terms of individual points and connected points   //
// (polylines).  The plotters can then blindly iterate through their        //
// plottables, obtaining their points, scaling them, and plotting them.     //
//                                                                          //
// The plottables also hold the plot styles                                 //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef _PLOTABLE_H_
#define _PLOTABLE_H_
#include <string>
class QRectF;
class AbsPlotter;
class Plotable {
  
public:

  // Constructor
  Plotable();

  // Destructor
  virtual ~Plotable();

  // Get the "natural rectangular border"
  virtual const QRectF & rectHint() const=0;

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo (AbsPlotter *plotter) const =0;

};
#endif

