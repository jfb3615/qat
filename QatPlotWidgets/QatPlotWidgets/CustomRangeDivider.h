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

#ifndef _CUSTOMRANGEDIVIDER_H_
#include "QatPlotWidgets/AbsRangeDivider.h"
#include "QatPlotWidgets/RangeDivision.h"
#include <vector>
#include <string>
class QFont;
class CustomRangeDivider : public AbsRangeDivider {

 public:
  // Constructor:
  CustomRangeDivider();

  // Destructor:
  ~CustomRangeDivider();

  // Insert a subdivision.  The label is cloned & the clone managed.
  void add (double x, QTextDocument *label=NULL);

  // Insert a subdivision.  The label is cloned & the clone managed.
  void add (double x, const std::string & label, const QFont * font=NULL);

  // Get the number of subdivision:
  virtual int getNumSubdivisions() const;

  // Get the location of each subdivision:
  virtual const RangeDivision & getSubdivision ( int i) const;

  // Three methods to set the range.  The calling the first method once
  // is more efficient than calling the last methods twice!
  virtual void setRange(double min, double max);
  virtual void setMin(double min);
  virtual void setMax(double max);

 // Get the validity of each subdivision:
  virtual bool isValid(int i) const;

 private:

  double _min;
  double _max;
  std::vector<RangeDivision> _subdivision;

};
#endif
