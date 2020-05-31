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

#include "QatPlotting/PlotShadeBox.h"
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
class PlotShadeBox::Clockwork {


public:

  Clockwork():myProperties(NULL) {}
  ~Clockwork() { delete myProperties;}
  
  QRectF                                nRectangle; 
  Properties                            *myProperties;
  Properties                            defaultProperties;
  std::vector<QPointF>                  points;
  std::vector<double>                   sizePlus;  
  std::vector<double>                   sizeMnus;
};


// Constructor
PlotShadeBox::PlotShadeBox():
  Plotable(),c(new Clockwork())
{
  c->nRectangle.setLeft  (+1.0E100);
  c->nRectangle.setRight (-1.0E100);
  c->nRectangle.setTop   (+1.0E100);
  c->nRectangle.setBottom(-1.0E100);
  
}



// Destructor
PlotShadeBox::~PlotShadeBox(){
  delete c;
}



const QRectF & PlotShadeBox::rectHint() const {
  return c->nRectangle;
}



  // Add Points:
void PlotShadeBox::addPoint(const QPointF & point, double errorPlus, double errorMinus) {
  c->points.push_back(point);
  c->sizePlus.push_back(errorPlus);
  c->sizeMnus.push_back(errorMinus);

  
  c->nRectangle.setLeft(std::min(c->nRectangle.left(),point.x()));
  c->nRectangle.setRight(std::max(c->nRectangle.right(),point.x()));
  c->nRectangle.setBottom(std::min(c->nRectangle.bottom(),point.y()+errorPlus));
  c->nRectangle.setTop(std::max(c->nRectangle.top(),point.y()-errorMinus));
}

void PlotShadeBox::addPoint( const QPointF & point, double size) {
  
  c->points.push_back(point);
  c->sizePlus.push_back(size);
  c->sizeMnus.push_back(size);

  
  c->nRectangle.setLeft(std::min(c->nRectangle.left(),point.x()));
  c->nRectangle.setRight(std::max(c->nRectangle.right(),point.x()));
  c->nRectangle.setBottom(std::min(c->nRectangle.bottom(),point.y()+size));
  c->nRectangle.setTop(std::max(c->nRectangle.top(),point.y()-size));
}


void PlotShadeBox::describeYourselfTo(AbsPlotter * plotter) const {

  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  double hSize=properties().horizontalSize;

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  
  for (unsigned int i=0;i<c->points.size();i++) {
    


    double  xdxp = c->points[i].x() + hSize/2;
    double  xdxm = c->points[i].x() - hSize/2;
    double  ydyp = c->points[i].y() + c->sizePlus[i];
    double  ydym = c->points[i].y() - c->sizeMnus[i];
    
   
    xdxp = plotter->isLogX() ? (*toLogX) (xdxp): xdxp;
    xdxm = plotter->isLogX() ? (*toLogX) (xdxm): xdxm;
    ydyp = plotter->isLogY() ? (*toLogY) (ydyp): ydyp;
    ydym = plotter->isLogY() ? (*toLogY) (ydym): ydym;
    
    if (ydyp<plotter->qrect()->top()) return;
    if (ydym>plotter->qrect()->bottom()) return;
    if (xdxp<plotter->qrect()->left()) return;
    if (xdxm>plotter->qrect()->right()) return;

    xdxp = std::min(xdxp,plotter->qrect()->right());
    xdxm = std::max(xdxm,plotter->qrect()->left());
    ydyp = std::min(ydyp,plotter->qrect()->bottom());
    ydym = std::max(ydym,plotter->qrect()->top());

    QRectF rect;
    rect.setTop(ydym);
    rect.setBottom(ydyp);
    rect.setLeft(xdxm);
    rect.setRight(xdxp);
    QGraphicsRectItem *shape = new QGraphicsRectItem(m.mapRect(rect));
        
    shape->setPen(pen);
    shape->setBrush(brush);
    shape->setMatrix(mInverse);
    plotter->scene()->addItem(shape);
    plotter->group()->addToGroup(shape);
  }

  delete toLogX;
  delete toLogY;
}

const PlotShadeBox::Properties & PlotShadeBox::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotShadeBox::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotShadeBox::resetProperties() {
  delete c->myProperties;
}

