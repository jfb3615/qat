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



#ifndef _PLOTKEY_H_
#define _PLOTKEY_H_
#include "QatPlotting/Plotable.h"
#include <QRectF>
#include <QString>
#include <QFont>
#include <string>

class QGraphicsTextItem;
class QTextDocument;

class PlotKey: public Plotable {

public:

   // Constructors
  PlotKey(double x, double y); 

  // Destructor
  virtual ~PlotKey();

  // Add a key for a plotable:
  void add(const Plotable *plotable, const std::string & label);

  // Get the font:
  QFont font() const;

  // Set the font:
  void setFont (const QFont & font);

  // Get the "natural rectangular border"
  virtual const QRectF & rectHint() const;

  // Describe to plotter, in terms of primitives:
  virtual void describeYourselfTo(AbsPlotter *plotter) const;

  // Copy constructor:
  PlotKey(const PlotKey &);
  
  // Assignment operator:
  PlotKey & operator=(const PlotKey &); 
  
private:

  class Clockwork;
  Clockwork *c;

};
#endif
