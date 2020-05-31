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

#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/AbsFunction.h"
#include "QatPlotting/AbsPlotter.h"

#include "QatDataAnalysis/Query.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsScene>
#include <stdexcept>


// Constructor
PlotFunction1D::PlotFunction1D(Genfun::GENFUNCTION function,
			       const QRectF & naturalRectangle):
  Plotable(),c(new Clockwork())
{
  c->function=function.clone();
  c->rect=naturalRectangle;
  c->domainRestriction=NULL;
}

PlotFunction1D::PlotFunction1D(Genfun::GENFUNCTION function,
			       const Cut<double> & domainRestriction,
			       const QRectF & naturalRectangle):
  Plotable(),c(new Clockwork()) 
{
  c->function=function.clone();
  c->rect=naturalRectangle;
  c->domainRestriction=domainRestriction.clone();
}

// Copy constructor:
PlotFunction1D::PlotFunction1D(const PlotFunction1D & source):
  Plotable(),c(new Clockwork())
{
  c->function=source.c->function->clone();
  c->rect=source.c->rect;
  c->domainRestriction=source.c->domainRestriction ? source.c->domainRestriction->clone() : NULL;
}

// Assignment operator:
PlotFunction1D & PlotFunction1D::operator=(const PlotFunction1D & source){
  if (&source!=this) {
    c->function=source.c->function->clone();
    c->rect=source.c->rect;
    
    delete c->domainRestriction;
    c->domainRestriction = source.c->domainRestriction ? source.c->domainRestriction->clone() : NULL;
  }
  return *this;
} 
 

#include <iostream>
// Destructor
PlotFunction1D::~PlotFunction1D(){
  delete c;
}



const QRectF & PlotFunction1D::rectHint() const {
  return c->rect;
}


// Get the graphic description::
void PlotFunction1D::describeYourselfTo(AbsPlotter *plotter) const {
  

  QPen pen = properties().pen;
  QBrush brush=properties().brush;


  QTransform m=plotter->matrix(),mInverse=m.inverted();

  unsigned int dim = c->function->dimensionality();
  if (dim!=1) throw std::runtime_error("PlotFunction1D:  requires a function of exactly 1 argument");
  
  double minX=plotter->qrect()->left(), maxX=plotter->qrect()->right(); 
  double minY=plotter->qrect()->top(),  maxY=plotter->qrect()->bottom();
 

  int NPOINTS = 200; 
  double delta = plotter->isLogX()? (log(maxX)-log(minX))/NPOINTS: (maxX-minX)/NPOINTS;

  const LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  const LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  
  if (brush.style()==Qt::NoBrush) {
    // Just a line:

    Query<QPointF>      cachedPoint;
    QPainterPath        *path=NULL; 
    bool                empty=true;
    
    for (int i=0; i<NPOINTS+1;i++) {
      bool                closePath=false;

      
      
      double x = toLogX ?  exp(log(minX)+i*delta) :minX + i*delta;
      double y=0;
      if ((!c->domainRestriction || (*c->domainRestriction)(x))  && std::isfinite((y=(*c->function)(x)))) {  // Check that X is in function domain 
	
	//double y = (*c->function) (x);
	QPointF point(x,y);
	if (y <= maxY && y >= minY) { // IN RANGE
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
	polyline->setTransform(mInverse);
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
      double x = toLogX ?  exp(log(minX)+i*delta) :minX + i*delta;
      double y=0;
      if ((!c->domainRestriction || (*c->domainRestriction)(x))  && std::isfinite((y=(*c->function)(x)))) {  // Check that X is in function domain 
	
	//double y = (*c->function) (x);
	QPointF point(x,y);

	if (!path) path = new QPainterPath();
	if (empty) {
	  empty=false;
	  Clockwork::moveTo(path,m, QPointF(x,yBase), toLogX,toLogY);
	}
	

	if (y <= maxY && y >= minY) { // IN RANGE
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
	polyline->setTransform(mInverse);
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


const PlotFunction1D::Properties & PlotFunction1D::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotFunction1D::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotFunction1D::resetProperties() {
  delete c->myProperties;
}
