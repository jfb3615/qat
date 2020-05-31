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



#include "QatPlotting/PlotProfile.h"
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

class PlotProfile::Clockwork {


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
PlotProfile::PlotProfile():
  Plotable(),c(new Clockwork())
{
  c->nRectangle.setLeft  (+1.0E100);
  c->nRectangle.setRight (-1.0E100);
  c->nRectangle.setTop   (+1.0E100);
  c->nRectangle.setBottom(-1.0E100);
  
}



// Destructor
PlotProfile::~PlotProfile(){
  delete c;
}



const QRectF & PlotProfile::rectHint() const {
  return c->nRectangle;
}


void PlotProfile::addPoint(double x, double y, double size) {
  addPoint(QPointF(x,y),size);
}
  // Add Points:
void PlotProfile::addPoint(const QPointF & point, double errorPlus, double errorMinus) {
  c->points.push_back(point);
  c->sizePlus.push_back(errorPlus);
  c->sizeMnus.push_back(errorMinus);

  
  c->nRectangle.setLeft(std::min(c->nRectangle.left(),point.x()));
  c->nRectangle.setRight(std::max(c->nRectangle.right(),point.x()));
  c->nRectangle.setBottom(std::min(c->nRectangle.bottom(),point.y()+errorPlus));
  c->nRectangle.setTop(std::max(c->nRectangle.top(),point.y()-errorMinus));
}

void PlotProfile::addPoint( const QPointF & point, double size) {
  
  c->points.push_back(point);
  c->sizePlus.push_back(size);
  c->sizeMnus.push_back(size);

  
  c->nRectangle.setLeft(std::min(c->nRectangle.left(),point.x()));
  c->nRectangle.setRight(std::max(c->nRectangle.right(),point.x()));
  c->nRectangle.setBottom(std::min(c->nRectangle.bottom(),point.y()+size));
  c->nRectangle.setTop(std::max(c->nRectangle.top(),point.y()-size));
}


void PlotProfile::describeYourselfTo(AbsPlotter * plotter) const {

  QPen pen =properties().pen;
  QBrush brush=properties().brush;
  int symbolSize=properties().symbolSize;
  int errorBarSize=properties().errorBarSize;
  Properties::SymbolStyle symbolStyle=properties().symbolStyle;
  bool drawSymbol=properties().drawSymbol;

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  
  for (unsigned int i=0;i<c->points.size();i++) {
    double x = plotter->isLogX() ? (*toLogX) (c->points[i].x()) : c->points[i].x();
    
    double y = plotter->isLogY() ? (*toLogY) (c->points[i].y()) : c->points[i].y();
    double  ydyp = plotter->isLogY() ? (*toLogY)(c->points[i].y() + c->sizePlus[i]) : c->points[i].y() + c->sizePlus[i];
    double  ydym = plotter->isLogY() ? (*toLogY)(c->points[i].y() - c->sizeMnus[i]) : c->points[i].y() - c->sizeMnus[i];
    
    QPointF loc(x, y );
    QSizeF  siz(symbolSize,symbolSize);
    QSizeF  esiz(errorBarSize,errorBarSize);
    QPointF ctr(siz.width()/2.0, siz.height()/2.0);
    QPointF had(esiz.width()/2.0, 0);
    QPointF vad(0,siz.height()/2.0);
    
    QPointF plus(x,ydyp);
    QPointF mnus(x,ydym);
    
    if (plotter->qrect()->contains(loc)) {
      QAbstractGraphicsShapeItem *shape=NULL;
      if (drawSymbol) {
	if (symbolStyle==Properties::CIRCLE) {
	  shape = new QGraphicsEllipseItem(QRectF(m.map(loc)-ctr,siz));
	}
	else if (symbolStyle==Properties::SQUARE) {
	  shape = new QGraphicsRectItem(QRectF(m.map(loc)-ctr,siz));
	}
	else if (symbolStyle==Properties::TRIANGLE_U) {
	  QVector<QPointF> points;
	  points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2,0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(0,symbolSize)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else if (symbolStyle==Properties::TRIANGLE_L) {
	  QVector<QPointF> points;
	  points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize,   0)));
	  points.push_back(m.map(loc)-ctr+(QPointF(symbolSize/2, symbolSize)));
	  points.push_back(m.map(loc)-ctr+(QPointF(0,            0)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else {
	  throw std::runtime_error("In PlotProfile:  unknown plot symbol");
	}
	shape->setPen(pen);
	shape->setBrush(brush);
	shape->setMatrix(mInverse);
	plotter->scene()->addItem(shape);
	plotter->group()->addToGroup(shape);
	
      }
      if (c->sizePlus[i]!=0.0&& c->sizeMnus[i]!=0.0) {

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
  delete toLogX;
  delete toLogY;
}

const PlotProfile::Properties & PlotProfile::properties() const { 
  return c->myProperties ? *c->myProperties : c->defaultProperties;
}

void PlotProfile::setProperties(const Properties &  properties) { 
  if (!c->myProperties) {
    c->myProperties = new Properties(properties);
  }
  else {
    *c->myProperties=properties;
  }
}

void PlotProfile::resetProperties() {
  delete c->myProperties;
}

