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



//----------------------------------------------------------------------------------------//
//                                                                                        //
//  Abstract base class for plotters                                                      //
//                                                                                        //
//----------------------------------------------------------------------------------------//

#ifndef _ABSPLOTTER_H_
#define _ABSPLOTTER_H_

class Graphic;
class QRectF;
class PRectF;
class Plotable;
class QMatrix;
class QGraphicsItemGroup;
class QGraphicsScene;
class AbsPlotter {

public:
 
  // Default constructor:
  AbsPlotter();

  // Virtual destructor:
  virtual ~AbsPlotter();

  // Add a plotable:
  virtual void add(Plotable *)=0;

  // Get the scene
  virtual QGraphicsScene *scene()=0;

  // Get the contents group:
  virtual QGraphicsItemGroup *group()=0;

  // Get the transformation matrix.
  virtual QMatrix matrix()=0;

  // Set the Plot Rectangle
  virtual void setRect(PRectF) = 0;

  // Get the Plot Rectangle:
  virtual PRectF * rect()  = 0; 
  virtual const PRectF * rect() const = 0; 

  // Clear the Plots, and the rectangle as well:
  virtual void clear() = 0;

  // Is this log scale X?
  virtual bool isLogX() const=0;

  // Is this log scale Y?
  virtual bool isLogY() const=0;

  // Start Animating
  virtual void startAnimating(int frameRateMillisec)=0;

  // Stop Animating 
  virtual void stopAnimating()=0;

  // Get the Plot Rectangle. For Internal use.
  virtual QRectF * qrect()  = 0; 
  virtual const QRectF * qrect() const = 0; 


};

#endif

