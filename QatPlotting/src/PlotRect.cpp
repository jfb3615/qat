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

#include "QatPlotting/PlotRect.h"
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

class PlotRect::Clockwork {

  public:

  Clockwork():myProperties(NULL) {}
  ~Clockwork() { delete myProperties;}



  QRectF                                rectangle;   
  Properties                            *myProperties;
  Properties                            defaultProperties;
};



PlotRect::PlotRect (const PlotRect & right):Plotable(),c(new Clockwork()){
  if (right.c->myProperties) {
    c->myProperties= new Properties(*right.c->myProperties);
  }
  c->rectangle=right.c->rectangle;
}

PlotRect & PlotRect::operator=(const PlotRect & right) {
  if (&right!=this) {
    if (right.c->myProperties) c->myProperties= new Properties(*right.c->myProperties);
    c->rectangle=right.c->rectangle;
  }
  return *this;
}

// Constructor
PlotRect::PlotRect(const QRectF & rectangle)
  :Plotable(),c(new Clockwork())

{
  c->rectangle=rectangle;

}



// Destructor
PlotRect::~PlotRect(){
  delete c;
}


// Get the "natural maximum R"
const QRectF & PlotRect::rectHint() const {
  return c->rectangle;
}



// Describe to plotter, in terms of primitives:
void PlotRect::describeYourselfTo(AbsPlotter *plotter) const{

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  double x0=c->rectangle.left();
  double x1=c->rectangle.right();
  double y0=c->rectangle.bottom();
  double y1=c->rectangle.top();

  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  if (toLogX) x0 = (*toLogX)(x0);
  if (toLogY) y0 = (*toLogY)(y0);
  if (toLogX) x1 = (*toLogX)(x1);
  if (toLogY) y1 = (*toLogY)(y1);

  QVector<QPointF> points;
  points.push_back(m.map(QPointF(x0,y0)));
  points.push_back(m.map(QPointF(x0,y1)));
  points.push_back(m.map(QPointF(x1,y1)));
  points.push_back(m.map(QPointF(x1,y0)));
  points.push_back(m.map(QPointF(x0,y0)));
  QGraphicsPolygonItem *shape = new QGraphicsPolygonItem(QPolygonF(points));
  
  shape->setPen(pen);
  shape->setBrush(brush);
  shape->setMatrix(mInverse);
  plotter->scene()->addItem(shape);
  plotter->group()->addToGroup(shape);
  

  delete toLogX;
  delete toLogY;
}

const PlotRect::Properties & PlotRect::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotRect::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotRect::resetProperties() {
  delete c->myProperties;
}

