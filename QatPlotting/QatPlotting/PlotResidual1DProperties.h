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

#ifndef _PLOTRESIDUAL1DPROPERTIES_H_
#define _PLOTRESIDUAL1DPROPERTIES_H_
#include <QPen>

class PlotResidual1DProperties {

 public:


  QPen                                              pen;

  enum SymbolStyle {CIRCLE, 
		    SQUARE,
		    TRIANGLE_U, 
		    TRIANGLE_L}                     symbolStyle;

  int                                               symbolSize;

  // Construct Default:
  PlotResidual1DProperties(): 
    symbolStyle(CIRCLE),
    symbolSize(5) 
      {}

};

#endif
