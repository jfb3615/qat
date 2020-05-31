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

#include "QatPlotting/PlotPoint.h"
#include "QatPlotting/LinToLog.h"
#include "QatPlotting/AbsPlotter.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <iostream>
#include <iomanip>
#include <sstream>

class PlotPoint::Clockwork {

  public:

  Clockwork():myProperties(NULL) {}
  ~Clockwork() { delete myProperties;}


  double                                x,y;
  QRectF                                nRectangle;   
  Properties                            *myProperties;
  Properties                            defaultProperties;
};



PlotPoint::PlotPoint (const PlotPoint & right):Plotable(),c(new Clockwork()){
  if (right.c->myProperties) {
    c->myProperties= new Properties(*right.c->myProperties);
  }
  c->x=right.c->x;
  c->y=right.c->y;
  c->nRectangle=right.c->nRectangle;
}

PlotPoint & PlotPoint::operator=(const PlotPoint & right) {
  if (&right!=this) {
    if (right.c->myProperties) c->myProperties= new Properties(*right.c->myProperties);
    c->x=right.c->x;
    c->y=right.c->y;
    c->nRectangle=right.c->nRectangle;
  }
  return *this;
}

// Constructor
PlotPoint::PlotPoint(double x, double y)
  :Plotable(),c(new Clockwork())

{
  c->x=x;
  c->y=y;
  
  c->nRectangle=QRectF(c->x-1E-10, c->y-1E-10, 2E-10, 2E-10);

}



// Destructor
PlotPoint::~PlotPoint(){
  delete c;
}


// Get the "natural maximum R"
const QRectF & PlotPoint::rectHint() const {
  return c->nRectangle;
}



// Describe to plotter, in terms of primitives:
void PlotPoint::describeYourselfTo(AbsPlotter *plotter) const{

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  double x=c->x;
  double y=c->y;

  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  int symbolSize=properties().symbolSize;
  PlotPoint::Properties::SymbolStyle symbolStyle=properties().symbolStyle;
  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  if (toLogX) x = (*toLogX)(x);
  if (toLogY) y = (*toLogY)(y);

  QPointF loc(x, y );
  QSizeF  siz(symbolSize,symbolSize);
  QPointF ctr(siz.width()/2.0, siz.height()/2.0);
  QPointF had(siz.width()/2.0, 0);
  QPointF vad(0,siz.height()/2.0);
      
  if (plotter->qrect()->contains(loc)) {
    QAbstractGraphicsShapeItem *shape=NULL;
    if (symbolStyle==PlotPoint::Properties::CIRCLE) {
      shape = new QGraphicsEllipseItem(QRectF(m.map(loc)-ctr,siz));
    }
    else if (symbolStyle==PlotPoint::Properties::SQUARE) {
      shape = new QGraphicsRectItem(QRectF(m.map(loc)-ctr,siz));
    }
    else if (symbolStyle==PlotPoint::Properties::TRIANGLE_U) {
      QVector<QPointF> points;
      points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
      points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,symbolSize)));
      points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2,0)));
      points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
      shape = new QGraphicsPolygonItem(QPolygonF(points));
    }
    else if (symbolStyle==PlotPoint::Properties::TRIANGLE_L) {
      QVector<QPointF> points;
      points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
      points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,   0)));
      points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2, symbolSize)));
      points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
      shape = new QGraphicsPolygonItem(QPolygonF(points));
    }
    else {
      throw std::runtime_error("In PlotPoint:  unknown plot symbol");
    }
    
    shape->setPen(pen);
    shape->setBrush(brush);
    shape->setMatrix(mInverse);
    plotter->scene()->addItem(shape);
    plotter->group()->addToGroup(shape);
    
  }

  delete toLogX;
  delete toLogY;
}

const PlotPoint::Properties & PlotPoint::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotPoint::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotPoint::resetProperties() {
  delete c->myProperties;
}

