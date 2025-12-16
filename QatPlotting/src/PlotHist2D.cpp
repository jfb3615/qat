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



#include "QatDataAnalysis/Hist2D.h"
#include "QatPlotting/PlotHist2D.h"
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

class PlotHist2D::Clockwork {
public:
  
  Clockwork() : histogram(NULL),
		myProperties(NULL) {}

  ~Clockwork() {delete myProperties;}
    
  // This is state:
  const Hist2D                          *histogram;         // The histogram
  QRectF                                 nRectangle;        // The "natural" bounding rectangle
  Properties                            *myProperties;      // My properties.  May 
  Properties                             defaultProperties;
};


PlotHist2D::PlotHist2D(const Hist2D & histogram):
  Plotable(),c(new Clockwork())
{

  c->histogram=&histogram;

  c->nRectangle.setLeft  (histogram.minX());
  c->nRectangle.setRight(histogram.maxX());
  c->nRectangle.setTop(histogram.minY());
  c->nRectangle.setBottom(histogram.maxY());
}

// Copy constructor:
PlotHist2D::PlotHist2D(const PlotHist2D & source):
  Plotable(),c(new Clockwork())
{
  c->histogram=source.c->histogram;
  c->nRectangle=source.c->nRectangle;
  if (source.c->myProperties) c->myProperties = new Properties(*source.c->myProperties);
  
}

// Assignment operator:
PlotHist2D & PlotHist2D::operator=(const PlotHist2D & source)
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
PlotHist2D::~PlotHist2D(){
  delete c;
}


const QRectF & PlotHist2D::rectHint() const {
  return c->nRectangle;
}


// Get the graphic description::
void PlotHist2D::describeYourselfTo(AbsPlotter *plotter) const {
  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  QTransform m=plotter->matrix(),mInverse=m.inverted();
  
  if (plotter->isLogX()) return;
  if (plotter->isLogY()) return;

  double hMinX=c->histogram->minX();
  double hMinY=c->histogram->minY();
  double hMaxX=c->histogram->maxX();
  double hMaxY=c->histogram->maxY();
  double max = c->histogram->maxContents(); 
  double binWidthX=c->histogram->binWidthX();
  double binWidthY=c->histogram->binWidthY();
  QPainterPath path;

  QRectF rect1=m.mapRect(*plotter->qrect());
  for (unsigned int i=0;i<c->histogram->nBinsX();i++) {
    for (unsigned int j=0;j<c->histogram->nBinsY();j++) {
      
      double bin = c->histogram->bin(i,j);
      if (bin==0) continue;
      
      double frac = bin/(max);
      double sqfrac = sqrt(frac);

      double x = c->histogram->binLowerEdgeX(i,j) + (1-sqfrac)*binWidthX/2.0;
      double y = c->histogram->binLowerEdgeY(i,j) + (1-sqfrac)*binWidthY/2.0 ;

      QRectF rect0(m.map(QPointF(x,y)),sqfrac*QSize(m.m11()*binWidthX,m.m22()*binWidthY));
      path.addRect(rect0&rect1);
    }
  }

  QAbstractGraphicsShapeItem *shape = new QGraphicsPathItem(path);
  shape->setPen(pen);
  shape->setBrush(brush);
  shape->setTransform(mInverse);

  plotter->scene()->addItem(shape);
  plotter->group()->addToGroup(shape);

}


// Get the histogram:
const Hist2D *PlotHist2D::histogram() const {
  return c->histogram;
}

const PlotHist2D::Properties & PlotHist2D::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotHist2D::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotHist2D::resetProperties() {
  delete c->myProperties;
}

