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
// Plottable adaptor for Function1D's                                               //
//                                                                                  //
//----------------------------------------------------------------------------------//
#ifndef _PLOTFUNCTION1D_H_
#define _PLOTFUNCTION1D_H_
#include "QatPlotting/Plotable.h"
#include "QatPlotting/PlotFunction1DProperties.h"
#include "QatGenericFunctions/CutBase.h"
#include <QRectF>
#include <vector>
#include <string>
#include <limits.h>
#include "QatGenericFunctions/AbsFunction.h"
namespace Genfun {
  class Parameter;
}

class PlotFunction1D: public Plotable{

public:

  // Define Properties:
  typedef PlotFunction1DProperties Properties;

   // Constructors
  PlotFunction1D(Genfun::GENFUNCTION function,
		 const Cut<double> & domainRestriction,
		 const QRectF   & rectHint = QRectF(QPointF(-10, -10), QSizeF(20, 20)));

   // Constructors
  PlotFunction1D(Genfun::GENFUNCTION function,
		 const QRectF   & rectHint = QRectF(QPointF(-10, -10), QSizeF(20, 20)));

  // Copy constructor:
  PlotFunction1D(const PlotFunction1D &);

  // Assignment operator:
  PlotFunction1D & operator=(const PlotFunction1D &); 

  // Destructor
  virtual ~PlotFunction1D();

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
#include "QatPlotting/PlotFunction1D.icc"
#endif
