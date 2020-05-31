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



#ifndef _PLOTPOINT_H_
#define _PLOTPOINT_H_
#include "QatPlotting/Plotable.h"
#include "QatPlotting/PlotPointProperties.h"
#include <QRectF>

class PlotPoint: public Plotable {

public:

  // Define Properties:
  typedef PlotPointProperties Properties;

   // Constructors
  PlotPoint(double x, double y); 

  // Destructor
  virtual ~PlotPoint();

  // Get the "natural rectangular border"
  virtual const QRectF & rectHint() const;

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo(AbsPlotter *plotter) const;

   // Set the properties
  void setProperties(const Properties &properties);

  // Revert to default properties:
  void resetProperties();

  // Get the properties (either default, or specific)
  const Properties &properties () const;
  
  // Copy constructor:
  PlotPoint(const PlotPoint &);
  
  // Assignment operator:
  PlotPoint & operator=(const PlotPoint &); 
  
private:

  class Clockwork;
  Clockwork *c;

};
#endif
