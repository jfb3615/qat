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

#include "QatDataAnalysis/Hist1D.h"
#include "QatPlotting/PlotResidual1D.h"
#include "QatPlotting/AbsPlotter.h"
#include "QatPlotting/LinToLog.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
// Constructor

class PlotResidual1D::Clockwork {
public:
  
  Clockwork() : histogram(NULL),
		myProperties(NULL) {}

  ~Clockwork() {delete myProperties;}
    
  // This is state:
  const Hist1D                          *histogram;         // The histogram
  const Genfun::AbsFunction             *function;          // The function
  QRectF                                 nRectangle;        // The "natural" bounding rectangle
  Properties                            *myProperties;      // My properties.  May 
  Properties                             defaultProperties;
};


PlotResidual1D::PlotResidual1D(const Hist1D & histogram, Genfun::GENFUNCTION function):
  Plotable(),c(new Clockwork())
{

  c->histogram=&histogram;
  c->function = function.clone();
  double yMin=FLT_MAX;
  double yMax=FLT_MIN;
  for (unsigned int i=0;i<histogram.nBins();i++) {
    double x      = histogram.binCenter(i);
    double yplus  = histogram.bin(i) -(*c->function)(x)+ histogram.binError(i);
    double yminus = histogram.bin(i) -(*c->function)(x)- histogram.binError(i);
    yMin = std::min(yMin,yminus);
    yMax = std::max(yMax,yplus);
  }   

  double dist = yMax-yMin;
  double marge = dist/10;
  yMax += marge;
  if (yMin>0) {
    yMin=0;
  }
  else if (yMin<0) {
    yMin-=marge;
  }

  QPointF lowerRight(histogram.min(),yMin);
  QPointF upperLeft (histogram.max(),yMax);
  c->nRectangle.setLeft  (histogram.min());
  c->nRectangle.setRight(histogram.max());
  c->nRectangle.setTop(yMin);
  c->nRectangle.setBottom(yMax);
}

// Copy constructor:
PlotResidual1D::PlotResidual1D(const PlotResidual1D & source):
  Plotable(),c(new Clockwork())
{
  
  c->histogram=source.c->histogram;
  c->function = source.c->function->clone();
  c->nRectangle=source.c->nRectangle;
  if (source.c->myProperties) c->myProperties = new Properties(*source.c->myProperties);
  
}

// Assignment operator:
PlotResidual1D & PlotResidual1D::operator=(const PlotResidual1D & source)
{
  if (&source!=this) {
    c->histogram=source.c->histogram;
    c->nRectangle=source.c->nRectangle;
    delete c->function;
    c->function = source.c->function->clone();
    delete c->myProperties;
    if (source.c->myProperties) {
      c->myProperties = new Properties(*source.c->myProperties);
    }
    else {
      c->myProperties=NULL;
    }
  }
  return *this;
} 


// Destructor
PlotResidual1D::~PlotResidual1D(){
  delete c->function;
  delete c;
}


const QRectF & PlotResidual1D::rectHint() const {
  return c->nRectangle;
}


// Get the graphic description::
void PlotResidual1D::describeYourselfTo(AbsPlotter *plotter) const {
  QPen pen =properties().pen;
  int symbolSize=properties().symbolSize;
  PlotResidual1D::Properties::SymbolStyle symbolStyle=properties().symbolStyle;
  
  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  QMatrix m=plotter->matrix(),mInverse=m.inverted();


  for (unsigned int i=0;i<c->histogram->nBins();i++) {
    double x = plotter->isLogX() ? (*toLogX) (c->histogram->binCenter(i)) : c->histogram->binCenter(i);
    double y = plotter->isLogY() ? (*toLogY) (c->histogram->bin(i)-(*c->function)(x)) : c->histogram->bin(i)-(*c->function)(x);
    double  ydyp = plotter->isLogY() ? (*toLogY)(c->histogram->bin(i)-(*c->function)(x)+c->histogram->binError(i)) : c->histogram->bin(i)-(*c->function)(x)+c->histogram->binError(i);
    double  ydym = plotter->isLogY() ? (*toLogY)(c->histogram->bin(i)-(*c->function)(x)-c->histogram->binError(i)) : c->histogram->bin(i)-(*c->function)(x)-c->histogram->binError(i);
    
    QPointF loc(x, y );
    QSizeF  siz(symbolSize,symbolSize);
    QPointF ctr(siz.width()/2.0, siz.height()/2.0);
    QPointF had(siz.width()/2.0, 0);
    QPointF vad(0,siz.height()/2.0);
    
    QPointF plus(x,ydyp);
    QPointF mnus(x,ydym);
    
    if (plotter->qrect()->contains(loc)) {
      QAbstractGraphicsShapeItem *shape=NULL;
      if (symbolStyle==PlotResidual1D::Properties::CIRCLE) {
	shape = new QGraphicsEllipseItem(QRectF(m.map(loc)-ctr,siz));
      }
      else if (symbolStyle==PlotResidual1D::Properties::SQUARE) {
	shape = new QGraphicsRectItem(QRectF(m.map(loc)-ctr,siz));
      }
      else if (symbolStyle==PlotResidual1D::Properties::TRIANGLE_U) {
	QVector<QPointF> points;
	points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,symbolSize)));
	points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2,0)));
	points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	shape = new QGraphicsPolygonItem(QPolygonF(points));
      }
      else if (symbolStyle==PlotResidual1D::Properties::TRIANGLE_L) {
	QVector<QPointF> points;
	points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,   0)));
	points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2, symbolSize)));
	points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	shape = new QGraphicsPolygonItem(QPolygonF(points));
      }
      else {
	throw std::runtime_error("In PlotResidual1D:  unknown plot symbol");
      }
      
      shape->setPen(pen);
      shape->setMatrix(mInverse);
      plotter->scene()->addItem(shape);
      plotter->group()->addToGroup(shape);
      
      {
	QLineF  pLine(m.map(loc)-vad, m.map(plus));
	if (plotter->qrect()->contains(plus)) {
	  QGraphicsLineItem *line=new QGraphicsLineItem(pLine);
	  line->setPen(pen);
	  line->setMatrix(mInverse);
	  plotter->scene()->addItem(line);
	  plotter->group()->addToGroup(line);
	  
	  QGraphicsLineItem *topLine=new QGraphicsLineItem(QLineF(m.map(plus)+had,m.map(plus)-had));
	  topLine->setPen(pen);
	  topLine->setMatrix(mInverse);
	  plotter->scene()->addItem(topLine);
	  plotter->group()->addToGroup(topLine);
	  
	}
	else {
	  QPointF intersection;
	  QLineF bottomLine(plotter->qrect()->bottomLeft(),plotter->qrect()->bottomRight());
	  QLineF::IntersectType type=bottomLine.intersect(QLineF(loc,plus),&intersection);
	  if (type==QLineF::BoundedIntersection) {
	    QPointF plus=intersection;
	    QLineF  pLine(m.map(loc)-vad, m.map(plus));
	    QGraphicsLineItem *line=new QGraphicsLineItem(pLine);
	    line->setPen(pen);
	    line->setMatrix(mInverse);
	    plotter->scene()->addItem(line);
	    plotter->group()->addToGroup(line);
	  }
	}
      }
      
      {
	QLineF  mLine(m.map(loc)+vad, m.map(mnus));
	if (plotter->qrect()->contains(mnus)) {
	  QGraphicsLineItem *line=new QGraphicsLineItem(mLine);
	  line->setPen(pen);
	  line->setMatrix(mInverse);
	  plotter->scene()->addItem(line);
	  plotter->group()->addToGroup(line);
	  
	  QGraphicsLineItem *bottomLine=new QGraphicsLineItem(QLineF(m.map(mnus)+had,m.map(mnus)-had));
	  bottomLine->setPen(pen);
	  bottomLine->setMatrix(mInverse);
	  plotter->scene()->addItem(bottomLine);
	  plotter->group()->addToGroup(bottomLine);
	  
	}
	else {
	  QPointF intersection;
	  QLineF topLine(plotter->qrect()->topLeft(),plotter->qrect()->topRight());
	  QLineF::IntersectType type=topLine.intersect(QLineF(loc,mnus),&intersection);
	  if (type==QLineF::BoundedIntersection) {
	    QPointF mnus=intersection;
	    QLineF  mLine(m.map(loc)+vad, m.map(mnus));
	    QGraphicsLineItem *line=new QGraphicsLineItem(mLine);
	    line->setPen(pen);
	    line->setMatrix(mInverse);
	    plotter->scene()->addItem(line);
	    plotter->group()->addToGroup(line);
	  }
	}
      }
      
      
    }
    else if (plotter->qrect()->contains(mnus)) {
      QPointF intersection;
      QLineF bottomLine(plotter->qrect()->bottomLeft(),plotter->qrect()->bottomRight());
      QLineF::IntersectType type=bottomLine.intersect(QLineF(loc,mnus),&intersection);
      if (type==QLineF::BoundedIntersection) {
	QPointF loc=intersection;
	QLineF  mLine(m.map(loc), m.map(mnus));
	QGraphicsLineItem *line=new QGraphicsLineItem(mLine);
	line->setPen(pen);
	line->setMatrix(mInverse);
	plotter->scene()->addItem(line);
	plotter->group()->addToGroup(line);
	
	QGraphicsLineItem *bottomLine=new QGraphicsLineItem(QLineF(m.map(mnus)+had,m.map(mnus)-had));
	bottomLine->setPen(pen);
	bottomLine->setMatrix(mInverse);
	plotter->scene()->addItem(bottomLine);
	plotter->group()->addToGroup(bottomLine);
	
      }
    }
    else if (plotter->qrect()->contains(plus)) {
      QPointF intersection;
      QLineF topLine(plotter->qrect()->topLeft(),plotter->qrect()->topRight());
      QLineF::IntersectType type=topLine.intersect(QLineF(loc,plus),&intersection);
      if (type==QLineF::BoundedIntersection) {
	QPointF loc=intersection;
	QLineF  pLine(m.map(loc), m.map(plus));
	QGraphicsLineItem *line=new QGraphicsLineItem(pLine);
	line->setPen(pen);
	line->setMatrix(mInverse);
	plotter->scene()->addItem(line);
	plotter->group()->addToGroup(line);
	
	QGraphicsLineItem *topLine=new QGraphicsLineItem(QLineF(m.map(plus)+had,m.map(plus)-had));
	topLine->setPen(pen);
	topLine->setMatrix(mInverse);
	plotter->scene()->addItem(topLine);
	plotter->group()->addToGroup(topLine);
	
      }
    }
  }
  delete toLogX;
  delete toLogY;
}

std::string PlotResidual1D::title() const {
  return c->histogram->name();
}

// Get the histogram:
const Hist1D *PlotResidual1D::histogram() const {
  return c->histogram;
}

// Get the function:
const Genfun::AbsFunction *PlotResidual1D::function() const {
  return c->function;
}

const PlotResidual1D::Properties & PlotResidual1D::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotResidual1D::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotResidual1D::resetProperties() {
  delete c->myProperties;
}

