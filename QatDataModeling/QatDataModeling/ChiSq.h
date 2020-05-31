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

#ifndef  _CHISQ_H_
#define  _CHISQ_H_
#include "QatDataModeling/ObjectiveFunction.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/AbsFunctional.h"
#include <vector>
class ChiSq:public Genfun::AbsFunctional {
public:

  // Constructor
  ChiSq(){}

  // Destructor:
  ~ChiSq(){}
  
  // Function Call Operator
  virtual double operator () (const Genfun::AbsFunction & f) const {
    double c2=0.0;
    for (unsigned int i=0;i<_points.size();i++) {
      point p = _points[i];
      double y = f(p.x);
      c2 += (p.y-y)*(p.y-y)/p.dy/p.dy;
    }
    return c2;
  }

  // Add a data point:
  void addPoint (double x, double y, double dy) {
    point p;
    p.x=x;
    p.y=y;
    p.dy=dy;
    _points.push_back(p);
  }
  
private:
  struct point {
    double x;
    double y;
    double dy;
  };
  
  std::vector<point> _points;

};
#endif
