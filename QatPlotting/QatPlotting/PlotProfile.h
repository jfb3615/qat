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



//----------------------------------------------------------------------------------//
//                                                                                  //
// Plottable Profile                                                                //
//                                                                                  //
//----------------------------------------------------------------------------------//
#ifndef _PLOTPROFILE_H_
#define _PLOTPROFILE_H_
#include "QatPlotting/Plotable.h"
#include "PlotProfileProperties.h"
#include <QRectF>
#include <vector>
class PlotProfile: public Plotable {

public:
  
  
  // Define properties:
  typedef PlotProfileProperties Properties;

  // Constructor
  PlotProfile();

  // Destructor
  virtual ~PlotProfile();

  // Get the "natural rectangular border"
  virtual const QRectF & rectHint() const;

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo(AbsPlotter *plotter) const;

  // Add Points:
  void addPoint(double x, double y, double size=0);

  // Add Points:
  void addPoint(const QPointF & point, double size=0);

  // Add Points:
  void addPoint(const QPointF & point, double errorPlus, double errorMinus);

 // Set the properties
  void setProperties(const Properties &properties);

  // Revert to default properties:
  void resetProperties();

  // Get the properties (either default, or specific)
  const Properties &properties () const;

private:

  // Copy constructor:
  PlotProfile(const PlotProfile &);

  // Assignment operator:
  PlotProfile & operator=(const PlotProfile &); 

  class Clockwork;
  Clockwork *c;


};
#endif
