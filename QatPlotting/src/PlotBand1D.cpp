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

#include "QatGenericFunctions/AbsFunction.h"
#include "QatPlotting/PlotBand1D.h"
#include "QatPlotting/AbsPlotter.h"
#include "QatPlotting/LinToLog.h"
#include "QatDataAnalysis/Query.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QtGlobal>
#include <stdexcept>
#include <tuple>
#include <vector>
class PlotBand1D::Clockwork {

public:

  Clockwork() :
    function1(NULL),
    function2(NULL),
    domainRestriction(NULL),
    myProperties(NULL) {}

  ~Clockwork() {
    delete domainRestriction;
    delete function1;
    delete function2;
  }

  static bool intersect(const QRectF * rect, const QPointF & p1, const QPointF & p2, QPointF & p) {

    double min=rect->top();
    double max=rect->bottom();
    double y1=p1.y();
    double y2=p2.y();

    if (rect->contains(p1) && rect->contains(p2)) {
      return false;
    }
    else if (y1 < min || y2 < min) {
#if QT_VERSION <= QT_VERSION_CHECK(5, 14, 0)
      if (QLineF(p1,p2).intersect(QLineF(rect->topLeft(),rect->topRight()),&p)!=QLineF::BoundedIntersection){
#else
      if (QLineF(p1,p2).intersects(QLineF(rect->topLeft(),rect->topRight()),&p)!=QLineF::BoundedIntersection){
#endif
	return false;
      }
    }
    else if (y1 > max || y2 > max) {
#if QT_VERSION <= QT_VERSION_CHECK(5, 14, 0)
      if (QLineF(p1,p2).intersect(QLineF(rect->bottomLeft(),rect->bottomRight()),&p)!=QLineF::BoundedIntersection){
#else
      if (QLineF(p1,p2).intersects(QLineF(rect->bottomLeft(),rect->bottomRight()),&p)!=QLineF::BoundedIntersection){
#endif
	return false;
      }
    }
    return true;
  }

  static bool maxOut(const QRectF * rect, const QPointF & p1, QPointF & p) {

    double min=rect->top();
    double max=rect->bottom();
    double y1=p1.y();

    if (rect->contains(p1)) {
      p=p1;
      return false;
    }

    if (y1>max) {
      p.setX(p1.x());
      p.setY(max);
    }
    else if (y1<min) {
      p.setX(p1.x());
      p.setY(min);
    }

    return true;
  }

  static void moveTo(QPainterPath *path, const QTransform & m, const QPointF & p, const LinToLog *linToLogX=NULL, const LinToLog *linToLogY=NULL) {
    double x=p.x(),y=p.y();
    if (linToLogX) x = (*linToLogX)(x);
    if (linToLogY) y = (*linToLogY)(y);
    
    path->moveTo(m.map(QPointF(x,y)));
  }

  static void lineTo(QPainterPath *path, const QTransform & m, const QPointF & p, const LinToLog *linToLogX=NULL, const LinToLog *linToLogY=NULL) {
    double x=p.x(),y=p.y();
    if (linToLogX) x = (*linToLogX)(x);
    if (linToLogY) y = (*linToLogY)(y);
    
    path->lineTo(m.map(QPointF(x,y)));
  }

  inline double transitionPoint(double x1, double x2) {
    if (!domainRestriction)          throw std::logic_error( "Do not call this routine if there is no domain restriction");
    bool b1=(*domainRestriction)(x1);
    bool b2=(*domainRestriction)(x2);
    if ((b1 && b2) || (!b1 && !b2))  return x1;
   
    double x = (x1+x2)/2.0;
    double tolerance=fabs(x1-x2)/100.0;
    while (fabs(x1-x2)>tolerance) {
      bool b = (*domainRestriction)(x);
      if (b == b1) {
	x1=x;
      }
      else if (b == b2) {
	x2=x;
      }
      x = (x1+x2)/2.0;
    }
    return x;
  }
  // This is state:
  const Genfun::AbsFunction            *function1;          // The function defining the band.
  const Genfun::AbsFunction            *function2;          // The function defining the band.
  QRectF                                rect;               // The "natural" bounding rectangle
  const Cut<double>                    *domainRestriction;  // Domain restrictions.
  Properties                           *myProperties;       // My properties.  May 
  Properties                            defaultProperties;

};

// Constructor
PlotBand1D::PlotBand1D(const Genfun::AbsFunction & function1,
		       const Genfun::AbsFunction & function2,
			       const QRectF & naturalRectangle):
  Plotable(),c(new Clockwork())
{
  c->function1=function1.clone();
  c->function2=function2.clone();
  c->rect=naturalRectangle;
  c->domainRestriction=NULL;
}

PlotBand1D::PlotBand1D(const Genfun::AbsFunction & function1,
		       const Genfun::AbsFunction & function2,
			       const Cut<double> & domainRestriction,
			       const QRectF & naturalRectangle):
  Plotable(),c(new Clockwork()) 
{
  c->function1=function1.clone();
  c->function2=function2.clone();
  c->rect=naturalRectangle;
  c->domainRestriction=domainRestriction.clone();
}

// Copy constructor:
PlotBand1D::PlotBand1D(const PlotBand1D & source):
  Plotable(),c(new Clockwork())
{
  c->function1=source.c->function1->clone();
  c->function2=source.c->function2->clone();
  c->rect=source.c->rect;
  c->domainRestriction=source.c->domainRestriction ? source.c->domainRestriction->clone() : NULL;
}

// Assignment operator:
PlotBand1D & PlotBand1D::operator=(const PlotBand1D & source){
  if (&source!=this) {
    c->function1=source.c->function1->clone();
    c->function2=source.c->function2->clone();
    c->rect=source.c->rect;
    
    delete c->domainRestriction;
    c->domainRestriction = source.c->domainRestriction ? source.c->domainRestriction->clone() : NULL;
  }
  return *this;
} 
 

#include <iostream>
// Destructor
PlotBand1D::~PlotBand1D(){
  delete c;
}



const QRectF & PlotBand1D::rectHint() const {
  return c->rect;
}


// Get the graphic description::
void PlotBand1D::describeYourselfTo(AbsPlotter *plotter) const {
  

  QPen pen = properties().pen;
  QBrush brush=properties().brush;


  QTransform m=plotter->matrix(),mInverse=m.inverted();

  {
    unsigned int dim = c->function1->dimensionality();
    if (dim!=1) throw std::runtime_error("PlotBand1D:  requires a function of exactly 1 argument");
  }  
  {
    unsigned int dim = c->function2->dimensionality();
    if (dim!=1) throw std::runtime_error("PlotBand1D:  requires a function of exactly 1 argument");
  }  

  double minX=plotter->qrect()->left(), maxX=plotter->qrect()->right(); 
  double minY=plotter->qrect()->top(),  maxY=plotter->qrect()->bottom();
 

  int NPOINTS = 200; 
  double delta = (maxX-minX)/NPOINTS;

  const LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  const LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;
  {
    // A fill pattern of some sort..
    QPainterPath        *path=NULL; 
    std::vector<std::tuple<double, double, double> > pVector;
    for (int i=0; i<NPOINTS+1;i++) {
      bool                closePath=false;
      double x = minX + i*delta;
      if (!c->domainRestriction || (*c->domainRestriction)(x)) {  // Check that X is in function domain 


	double y1 = (*c->function1) (x);
	double y2 = (*c->function2) (x) ;
	y1=std::max(minY,std::min(maxY,y1));
	y2=std::max(minY,std::min(maxY,y2));

	if (!path) {
	  path = new QPainterPath();
	  if (c->domainRestriction && x!=minX) {
	    double xtransition=c->transitionPoint(x-delta,x);
	    double y1 = (*c->function1) (xtransition);
	    double y2 = (*c->function2) (xtransition) ;
	    y1=std::max(minY,std::min(maxY,y1));
	    y2=std::max(minY,std::min(maxY,y2));
	    pVector.push_back(std::make_tuple(xtransition,y1,y2));
	  }
	}
	pVector.push_back(std::make_tuple(x,y1,y2));

      }
      else {   // OUT OF DOMAIN 
	if (path) {
	  if (c->domainRestriction) {
	    double xtransition=c->transitionPoint(x-delta,x);
	    double y1 = (*c->function1) (xtransition);
	    double y2 = (*c->function2) (xtransition) ;
	    y1=std::max(minY,std::min(maxY,y1));
	    y2=std::max(minY,std::min(maxY,y2));
	    pVector.push_back(std::make_tuple(xtransition,y1,y2));
	  }
	  closePath=true;
	}
      }
      if (i==NPOINTS && path) {
	closePath=true;
      }
      if (closePath) {
	Clockwork::moveTo(path,m, QPointF(std::get<0>(pVector[0]),std::get<1>(pVector[0])), toLogX, toLogY);
	for (size_t p=1;p<pVector.size();p++) {
	  Clockwork::lineTo(path,m, QPointF(std::get<0>(pVector[p]),std::get<1>(pVector[p])), toLogX, toLogY);
	};
	for (size_t p=pVector.size()-1;int(p)>=0;p--) {
	  Clockwork::lineTo(path,m, QPointF(std::get<0>(pVector[p]),std::get<2>(pVector[p])), toLogX, toLogY);
	};
	Clockwork::lineTo(path,m, QPointF(std::get<0>(pVector[0]),std::get<1>(pVector[0])), toLogX, toLogY);

	
	pVector.clear();
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



const PlotBand1D::Properties & PlotBand1D::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotBand1D::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotBand1D::resetProperties() {
  delete c->myProperties;
}
