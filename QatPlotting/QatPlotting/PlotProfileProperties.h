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

#ifndef _PLOTPROFILEPROPERTIES_H_
#define _PLOTPROFILEPROPERTIES_H_
#include <QPen>
#include <QBrush>

class PlotProfileProperties {

 public:


  QPen                                              pen;
  QBrush                                            brush;
  enum SymbolStyle {CIRCLE, 
		    SQUARE,
		    TRIANGLE_U, 
		    TRIANGLE_L}                     symbolStyle;

  int                                               symbolSize;
  int                                               errorBarSize;
  bool                                              drawSymbol;

  // Construct Default:
  PlotProfileProperties(): 
    symbolStyle(CIRCLE),
    symbolSize(5),
    errorBarSize(symbolSize),
    drawSymbol(true){
  }

};

#endif
