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

// ------------------------------------------------------------------------//
//                                                                         //
// This object represents a wave:  a plotable function of both position    //
// and time.  The resulting object is animate when placed into a PlotView. //
//                                                                         //
// The constructor takes an AbsFunction, which must be a function of two   //
// variables (Position, and Time).                                         //
//                                                                         //
//-------------------------------------------------------------------------//

#include "QatGenericFunctions/AbsFunction.h"
#include "QatPlotting/PlotWave1D.h"
#include "QatPlotting/AbsPlotter.h"
#include "QatPlotting/LinToLog.h"
#include "QatDataAnalysis/Query.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsScene>
#include <stdexcept>
#include <sys/time.h>

// Constructor
PlotWave1D::PlotWave1D(const Genfun::AbsFunction & function,
			       const QRectF & naturalRectangle):
  PlotFunction1D(function, naturalRectangle) {}


PlotWave1D::PlotWave1D(const Genfun::AbsFunction & function,
			       const Cut<double> & domainRestriction,
			       const QRectF & naturalRectangle):
  PlotFunction1D(function, domainRestriction, naturalRectangle) {}

// Copy constructor:
PlotWave1D::PlotWave1D(const PlotWave1D & source):
  PlotFunction1D(source) {}

// Assignment operator:
PlotWave1D & PlotWave1D::operator=(const PlotWave1D & source){
  if (&source !=this) {
    PlotFunction1D::operator=(source);
  }
  return *this;
} 
 

// Destructor
PlotWave1D::~PlotWave1D(){
}


// Get the graphic description::
void PlotWave1D::describeYourselfTo(AbsPlotter *plotter) const {
  
  plotter->startAnimating(20);

  Genfun::Argument arg(2);
  timeval tv;
  gettimeofday(&tv,NULL);
  arg[1] = double(tv.tv_sec) + tv.tv_usec/double(1000000);



  QPen pen = properties().pen;
  QBrush brush=properties().brush;


  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  unsigned int dim = c->function->dimensionality();
  if (dim!=2) throw std::runtime_error("PlotWave1D:  requires a function of exactly 2 arguments");
  
  double minX=plotter->qrect()->left(), maxX=plotter->qrect()->right(); 
  double minY=plotter->qrect()->top(),  maxY=plotter->qrect()->bottom();
 

  int NPOINTS = 200; 
  double delta = (maxX-minX)/NPOINTS;

  const LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  const LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  if (brush.style()==Qt::NoBrush) {
    // Just a line:

    Query<QPointF>      cachedPoint;
    QPainterPath        *path=NULL; 
    bool                empty=true;
    for (int i=0; i<NPOINTS+1;i++) {
      bool                closePath=false;
      double x = minX + i*delta;
      arg[0] = x;
      double y=0;
      if ((!c->domainRestriction || (*c->domainRestriction)(x))  && std::isfinite((y=(*c->function)(arg)))) {  // Check that X is in function domain 
	
	//double y = (*c->function) (x);
	QPointF point(x,y);
	if (y < maxY && y > minY) { // IN RANGE
	  if (!path) path = new QPainterPath();
	  if (empty) {
	    empty=false;
	    if (cachedPoint.isValid()) {
	      QPointF intersection;
	      if (Clockwork::intersect(plotter->qrect(),cachedPoint, point,intersection)) {
		Clockwork::moveTo(path,m,intersection,toLogX,toLogY);
		Clockwork::lineTo(path,m,point,toLogX,toLogY);
	      }
	      else {
		if (Clockwork::maxOut(plotter->qrect(), point, intersection)) {
		  Clockwork::moveTo(path,m,intersection,toLogX,toLogY);
		  Clockwork::lineTo(path,m,point,toLogX,toLogY);
		}
		else {
		  std::cerr << "Intersection is failing" << std::endl;
		}
	      }
	    }
	    else {
	      Clockwork::moveTo(path,m, point,toLogX,toLogY);
	    }
	  }
	  else {
	    Clockwork::lineTo(path,m,point,toLogX, toLogY);
	  }
	}	
	else { // OUT OF RANGE
	  if (path) {
	    if (cachedPoint.isValid()) {
	      QPointF intersection;
	      if (Clockwork::intersect(plotter->qrect(),cachedPoint, point,intersection)) {
		Clockwork::lineTo(path,m,intersection,toLogX,toLogY);
	      }
	      else {
		if (Clockwork::maxOut(plotter->qrect(), point, intersection)) {
		  Clockwork::moveTo(path,m,intersection,toLogX,toLogY);
		  Clockwork::lineTo(path,m,point,toLogX,toLogY);
		}
		else {
		  std::cerr << "Intersection is failing" << std::endl;
		}
	      }
	    }
	    closePath=true;
	    empty    =true;
	  }
	}
	cachedPoint=point;
      }
      else {   // OUT OF DOMAIN 
	if (path) {
	  closePath=true;
	  empty=true;
	}
	cachedPoint=Query<QPointF>();
      }
      if (i==NPOINTS && path) closePath=true;
      if (closePath) {
	QGraphicsPathItem *polyline=new QGraphicsPathItem(*path);
	polyline->setPen(pen);
	polyline->setBrush(brush);
	polyline->setMatrix(mInverse);
	plotter->scene()->addItem(polyline);
	plotter->group()->addToGroup(polyline);
	delete path;
	path=NULL;
      }
    }
  }
  else {
    // A fill pattern of some sort..
    double yBase=std::max(std::min(maxY,  properties().baseLine),minY);
     
    QPainterPath        *path=NULL; 
    bool                empty=true;
    for (int i=0; i<NPOINTS+1;i++) {
      bool                closePath=false;
      double x = minX + i*delta;
      arg[0]=x;
      double y=0;
      if ((!c->domainRestriction || (*c->domainRestriction)(x))  && std::isfinite((y=(*c->function)(arg)))) {  // Check that X is in function domain 
	
	//double y = (*c->function) (x);
	QPointF point(x,y);

	if (!path) path = new QPainterPath();
	if (empty) {
	  empty=false;
	  QPointF intersection;
	  Clockwork::moveTo(path,m, QPointF(x,yBase), toLogX,toLogY);
	}
	

	if (y < maxY && y > minY) { // IN RANGE
	  Clockwork::lineTo(path,m,point,toLogX,toLogY);
	}	
	else { // OUT OF RANGE
	  QPointF maxOut;
	  Clockwork::maxOut(plotter->qrect(), point,maxOut);
	  Clockwork::lineTo(path,m,maxOut,toLogX,toLogY);
	}
      }
      else {   // OUT OF DOMAIN 
	if (path) {
	  closePath=true;
	  empty=true;
	}
      }
      if (i==NPOINTS && path) {
	closePath=true;
      }
      if (closePath) {
	QPointF position=mInverse.map(path->currentPosition());
	position.setY(yBase);
	Clockwork::lineTo(path,m,position,toLogX,toLogY);
	QGraphicsPathItem *polyline=new QGraphicsPathItem(*path);
	polyline->setPen(pen);
	polyline->setBrush(brush);
	polyline->setMatrix(mInverse);
	plotter->scene()->addItem(polyline);
	plotter->group()->addToGroup(polyline);
	delete path;
	path=NULL;
      }
    }
  }
  delete toLogX;
  delete toLogY;
}



