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

#ifndef _PRectF_h_
#define _PRectF_h_
#include <QRectF>
class PRectF {

 public:
  
  // Constructors
  PRectF(): rect(0,0,1,1){};
  PRectF(const QRectF & rect):rect(rect){};
  PRectF(double xMin, double xMax, double yMin, double yMax):
  rect(xMin, yMin, xMax-xMin, yMax-yMin){}

  // Accessors
  double xmin() const {return rect.left();}
  double xmax() const {return rect.right();}
  double ymin() const {return rect.top();}
  double ymax() const {return rect.bottom();}
  
  // Modifiers
  void setXmin(double val) {rect.setLeft(val);}
  void setXmax(double val) {rect.setRight(val);}
  void setYmin(double val) {rect.setTop(val);}
  void setYmax(double val) {rect.setBottom(val);}
  
  operator QRectF() const {return rect;}

 private:

  QRectF rect;
};
#endif
