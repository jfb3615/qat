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
#include "QatPlotting/PlotHist1D.h"
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
#include <cmath>
// Constructor

class PlotHist1D::Clockwork {
public:
  
  Clockwork() : histogram(NULL),
		myProperties(NULL) {}

  ~Clockwork() {delete myProperties;}
    
  // This is state:
  const Hist1D                          *histogram;         // The histogram
  QRectF                                 nRectangle;        // The "natural" bounding rectangle
  Properties                            *myProperties;      // My properties.  May 
  Properties                             defaultProperties;
};


PlotHist1D::PlotHist1D(const Hist1D & histogram):
  Plotable(),c(new Clockwork())
{

  c->histogram=&histogram;

  double yMin=FLT_MAX;
  double yMax=FLT_MIN;
  for (unsigned int i=0;i<histogram.nBins();i++) {
    double yplus  = histogram.bin(i) + histogram.binError(i);
    double yminus = histogram.bin(i) - histogram.binError(i);
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

  QPointF lowerRight(c->histogram->min(),yMin);
  QPointF upperLeft (c->histogram->max(),yMax);
  c->nRectangle.setLeft  (histogram.min());
  c->nRectangle.setRight(histogram.max());
  c->nRectangle.setTop(yMin);
  c->nRectangle.setBottom(yMax);
}

// Copy constructor:
PlotHist1D::PlotHist1D(const PlotHist1D & source):
  Plotable(),c(new Clockwork())
{
  c->histogram=source.c->histogram;
  c->nRectangle=source.c->nRectangle;
  if (source.c->myProperties) c->myProperties = new Properties(*source.c->myProperties);
  
}

// Assignment operator:
PlotHist1D & PlotHist1D::operator=(const PlotHist1D & source)
{
  if (&source!=this) {
    c->histogram=source.c->histogram;
    c->nRectangle=source.c->nRectangle;
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
PlotHist1D::~PlotHist1D(){
  delete c;
}


const QRectF & PlotHist1D::rectHint() const {

  double yMin=FLT_MAX;
  double yMax=FLT_MIN;
  for (unsigned int i=0;i<c->histogram->nBins();i++) {
    double yplus  = c->histogram->bin(i) + c->histogram->binError(i);
    double yminus = c->histogram->bin(i) - c->histogram->binError(i);
    yMin = std::min(yMin,yminus);
    yMax = std::max(yMax,yplus);
  }   

  c->nRectangle.setLeft  (c->histogram->min());
  c->nRectangle.setRight(c->histogram->max());
  c->nRectangle.setTop(yMin);
  c->nRectangle.setBottom(yMax);
  return c->nRectangle;
}


// Get the graphic description::
void PlotHist1D::describeYourselfTo(AbsPlotter *plotter) const {
  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  int symbolSize=properties().symbolSize;
  PlotHist1D::Properties::SymbolStyle symbolStyle=properties().symbolStyle;
  
  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  if (properties().plotStyle==PlotHist1DProperties::SYMBOLS) {

    for (unsigned int i=0;i<c->histogram->nBins();i++) {
      double x = plotter->isLogX() ? (*toLogX) (c->histogram->binCenter(i)) : c->histogram->binCenter(i);
      double y = plotter->isLogY() ? (*toLogY) (c->histogram->bin(i)) : c->histogram->bin(i);
      double  ydyp = plotter->isLogY() ? (*toLogY)(c->histogram->bin(i)+c->histogram->binError(i)) : c->histogram->bin(i)+c->histogram->binError(i);
      double  ydym = plotter->isLogY() ? (*toLogY)(c->histogram->bin(i)-c->histogram->binError(i)) : c->histogram->bin(i)-c->histogram->binError(i);
      
      if (plotter->isLogY() && !std::isfinite(ydym)) ydym=plotter->qrect()->top()+FLT_MIN;

      QPointF loc(x, y );
      QSizeF  siz(symbolSize,symbolSize);
      QPointF ctr(siz.width()/2.0, siz.height()/2.0);
      QPointF had(siz.width()/2.0, 0);
      QPointF vad(0,siz.height()/2.0);
      
      QPointF plus(x,ydyp);
      QPointF mnus(x,ydym);
      
      if (plotter->qrect()->contains(loc)) {
	QAbstractGraphicsShapeItem *shape=NULL;
	if (symbolStyle==PlotHist1D::Properties::CIRCLE) {
	  shape = new QGraphicsEllipseItem(QRectF(m.map(loc)-ctr,siz));
	}
	else if (symbolStyle==PlotHist1D::Properties::SQUARE) {
	  shape = new QGraphicsRectItem(QRectF(m.map(loc)-ctr,siz));
	}
	else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_U) {
	  QVector<QPointF> points;
	  points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2,0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_L) {
	  QVector<QPointF> points;
	  points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,   0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2, symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else {
	  throw std::runtime_error("In PlotHist1D:  unknown plot symbol");
	}
	  
	shape->setPen(pen);
	shape->setBrush(brush);
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
  }
  else { // LINES
      
    QPainterPath path;

    double maxY=plotter->qrect()->bottom();
    double minY=plotter->qrect()->top();
    double minX=plotter->qrect()->left();
    double maxX=plotter->qrect()->right();
    bool started=false;
    for (unsigned int i=0;i<c->histogram->nBins();i++) {
      double y = plotter->isLogY() ? (*toLogY) (c->histogram->bin(i)) : c->histogram->bin(i);
      if (plotter->isLogY() && !std::isfinite(y)) y=minY;
      double binLow = c->histogram->binLowerEdge(i);
      double binHi  = c->histogram->binUpperEdge(i);
      if (toLogX) binLow=(*toLogX)(binLow);
      if (toLogX) binHi =(*toLogX)(binHi);


      double yEff=std::max(std::min(maxY,  y),minY);
      double zEff=std::max(std::min(maxY,0.0),minY);
      QPointF lowerPoint(binLow, yEff);
      QPointF upperPoint(binHi,  yEff);
      if ( binHi >= minX && binLow <= maxX ) {
	if (!started) {
	  started=true;
	  if (binLow > minX) {
	    path.moveTo(m.map(QPointF(binLow,zEff)));
	    path.lineTo(m.map(lowerPoint));
	    path.lineTo(m.map(upperPoint));
	  }
	  else {
	    path.moveTo(m.map(QPointF(minX,zEff)));
	    path.lineTo(m.map(QPointF(minX,yEff)));
	    path.lineTo(m.map(upperPoint));
	  }
	}
	else if (binHi>=maxX) {
	  path.lineTo(m.map(lowerPoint));
	  path.lineTo(m.map(QPointF(maxX,yEff)));
	  path.lineTo(m.map(QPointF(maxX,zEff)));
	}
	else {
	  path.lineTo(m.map(lowerPoint));
	  path.lineTo(m.map(upperPoint));
	}
	// AND:
	if (i==c->histogram->nBins()-1) {
	  path.lineTo(m.map(QPointF(binHi,zEff)));
	}
      }
    }
    QGraphicsPathItem *polyline=new QGraphicsPathItem(path);
    polyline->setPen(pen);
    polyline->setBrush(brush);
    polyline->setMatrix(mInverse);
    plotter->scene()->addItem(polyline);
    plotter->group()->addToGroup(polyline);
  }

  delete toLogX;
  delete toLogY;
}

std::string PlotHist1D::title() const {
  return c->histogram->name();
}

std::string PlotHist1D::textSummary() const {
    std::ostringstream textSummaryStream;
    textSummaryStream << std::setprecision(3) << std::showpoint << std::setfill(' ');
    textSummaryStream << "Area: "   << std::setw(9)        << c->histogram->sum()             << '\t'
		      << "Mean: "   << std::setw(9)        << c->histogram->mean()            << '\t'
		      << "Sigma:"   << std::setw(8)        << sqrt(c->histogram->variance())  << '\t';
    return textSummaryStream.str();
}

// Get the histogram:
const Hist1D *PlotHist1D::histogram() const {
  return c->histogram;
}

const PlotHist1D::Properties & PlotHist1D::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotHist1D::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotHist1D::resetProperties() {
  delete c->myProperties;
}

