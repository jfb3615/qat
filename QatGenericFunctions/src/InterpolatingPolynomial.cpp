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

#include "QatGenericFunctions/InterpolatingPolynomial.h"
#include <cmath>
#include <cfloat>
#include <stdexcept>
namespace Genfun {
  FUNCTION_OBJECT_IMP(InterpolatingPolynomial)
  
  InterpolatingPolynomial::InterpolatingPolynomial()
  :Genfun::AbsFunction(),delta(0)
  {}
  
  InterpolatingPolynomial::InterpolatingPolynomial(const InterpolatingPolynomial & right)
    :Genfun::AbsFunction(),xPoints(right.xPoints),q(right.q),delta(0)
  {}
  
  InterpolatingPolynomial::~InterpolatingPolynomial() {
  }
  
  double InterpolatingPolynomial::operator() (double x) const {

    if (xPoints.size()==0) throw std::runtime_error("InterpolatingPolynomial: no interpolation points");
    delta=HUGE_VAL;

    for (unsigned int i=0;i<xPoints.size();i++) q[i]=xPoints[i].second;
    double o=HUGE_VAL;
    for (unsigned int i=1;i<xPoints.size();i++) {
      for (unsigned j=xPoints.size()-1;j>=i;j--) {
      	q[j]=((x-xPoints[j].first)*q[j-1]-(x-xPoints[j-i].first)*q[j])/(xPoints[j-i].first -xPoints[j].first);
      }
      delta=q.back()-o;
      o=q.back();
    }
    return q.back();

  }
  
  void InterpolatingPolynomial::addPoint( double x, double y) {
    xPoints.push_back(std::make_pair(x,y));
    q.resize(xPoints.size());
  }

  double InterpolatingPolynomial::getDelta() const {
    return fabs(delta);
  }

  void InterpolatingPolynomial::getRange( double & min, double & max) const {
    min=DBL_MAX, max=-DBL_MAX;
    for (unsigned int i=0;i<xPoints.size();i++) {
      min = std::min(min,xPoints[i].first);
      max = std::max(max,xPoints[i].first);
    }
  }
} // namespace Genfun
