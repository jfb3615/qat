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
// Plottable adaptor for two Function1D's                                           //
//                                                                                  //
//----------------------------------------------------------------------------------//
#ifndef _PLOTORBIT_H_
#define _PLOTORBIT_H_
class AbsFunction;
#include "QatPlotting/Plotable.h"
#include "QatPlotting/PlotOrbitProperties.h"
#include "QatGenericFunctions/CutBase.h"
#include <QRectF>
#include <vector>
#include <string>
#include <limits.h>

namespace Genfun {
  class AbsFunction;
  class Parameter;
}

class PlotOrbit: public Plotable{

public:

  // Define Properties:
  typedef PlotOrbitProperties Properties;

   // Constructors
  PlotOrbit(const Genfun::AbsFunction & functionX, 
	    const Genfun::AbsFunction & functionY,
	    double t0,
	    double t1);

  // Copy constructor:
  PlotOrbit(const PlotOrbit &);

  // Assignment operator:
  PlotOrbit & operator=(const PlotOrbit &); 

  // Destructor
  virtual ~PlotOrbit();

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
  
protected:

  class Clockwork;
  Clockwork *c;
  

};
#include "QatPlotting/PlotOrbit.icc"
#endif
