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

#include "QatPlotting/PlotKey.h"
#include "QatPlotting/LinToLog.h"
#include "QatPlotting/AbsPlotter.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotOrbit.h"
#include "QatPlotting/PlotErrorEllipse.h"
#include "QatPlotting/PlotPoint.h"
#include "QatPlotting/PlotHist1D.h"
#include <QGraphicsPathItem>
#include <QTextCursor>
#include <QPainterPath>
#include <QSizeF>
#include <QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QTextDocument>
#include <iostream>
#include <iomanip>
#include <sstream>

class PlotKey::Clockwork {

  public:

  Clockwork():textDocument() {}
  ~Clockwork() {
    delete textDocument;
    delete nRect;
  }


  QPointF                               point;
  QTextDocument                         *textDocument;
  QRectF                                *nRect;
  std::vector<const Plotable *>         plotable;
};



PlotKey::PlotKey (const PlotKey & right):Plotable(),c(new Clockwork()){
  c->point=right.c->point;
  c->textDocument=right.c->textDocument->clone();
  c->nRect=NULL;
}

PlotKey & PlotKey::operator=(const PlotKey & right) {
  if (&right!=this) {
    c->point=right.c->point;
    c->textDocument=right.c->textDocument->clone();
    c->nRect=NULL;
  }
  return *this;
}

// Constructor
PlotKey::PlotKey(double x, double y)
  :Plotable(),c(new Clockwork())

{
  c->point=QPointF(x,y);
  c->textDocument=new QTextDocument("");
  c->nRect=NULL;
}



// Destructor
PlotKey::~PlotKey(){
  delete c;
}


// Get the "natural maximum R"
const QRectF & PlotKey::rectHint() const {
  if (!c->nRect) {
    QGraphicsTextItem aux;
    aux.setDocument(c->textDocument);
    aux.setPos(c->point);
    aux.adjustSize();
    c->nRect = new QRectF( aux.boundingRect());
  }
  return *c->nRect;
}



// Describe to plotter, in terms of primitives:
void PlotKey::describeYourselfTo(AbsPlotter *plotter) const{

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  double x=c->point.x();
  double y=c->point.y();

  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  if (toLogX) x = (*toLogX)(x);
  if (toLogY) y = (*toLogY)(y);



  QGraphicsTextItem *item = new QGraphicsTextItem();

  QPointF P(plotter->qrect()->left(), plotter->qrect()->bottom());
  QPointF Q(QPointF(x,y)), MQ=m.map(Q);

  item->setDocument(c->textDocument);
  item->setPos(Q-P);
  item->setMatrix(mInverse);
  plotter->scene()->addItem(item);
  plotter->group()->addToGroup(item);

  QFont font=c->textDocument->defaultFont();
  QFontMetrics fontMetrics(font);
  int lineSpacing=fontMetrics.lineSpacing();
  int charWidth=fontMetrics.width(' ');
  int count=0;
  for (const Plotable * p : c->plotable) {
    const PlotProfile *profile=dynamic_cast<const PlotProfile *> (p);
    if (profile) {
      QPointF ctr(-5*charWidth,-(0.65+count++)*lineSpacing);
      PlotProfile::Properties properties=profile->properties();
      
      QPen pen =properties.pen;
      QBrush brush=properties.brush;
      int symbolSize=properties.symbolSize;
      QSizeF siz(symbolSize,symbolSize);
      PlotProfile::Properties::SymbolStyle symbolStyle=properties.symbolStyle;
      bool drawSymbol=properties.drawSymbol;
      QAbstractGraphicsShapeItem *shape=NULL;
      if (drawSymbol) {
	if (symbolStyle==PlotProfile::Properties::CIRCLE) {
	  shape = new QGraphicsEllipseItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotProfile::Properties::SQUARE) {
	  shape = new QGraphicsRectItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotProfile::Properties::TRIANGLE_U) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2,0)));
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else if (symbolStyle==PlotProfile::Properties::TRIANGLE_L) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,   0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2, symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
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
      continue;
    }
    const PlotPoint *point=dynamic_cast<const PlotPoint *> (p);
    if (point) {
      QPointF ctr(-5*charWidth,-(0.65+count++)*lineSpacing);
      PlotPoint::Properties properties=point->properties();
      
      QPen pen =properties.pen;
      QBrush brush=properties.brush;
      int symbolSize=properties.symbolSize;
      QSizeF siz(symbolSize,symbolSize);
      PlotPoint::Properties::SymbolStyle symbolStyle=properties.symbolStyle;
      QAbstractGraphicsShapeItem *shape=NULL;
      {
	if (symbolStyle==PlotPoint::Properties::CIRCLE) {
	  shape = new QGraphicsEllipseItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotPoint::Properties::SQUARE) {
	  shape = new QGraphicsRectItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotPoint::Properties::TRIANGLE_U) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2,0)));
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else if (symbolStyle==PlotPoint::Properties::TRIANGLE_L) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,   0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2, symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	shape->setPen(pen);
	shape->setBrush(brush);
	shape->setMatrix(mInverse);
	plotter->scene()->addItem(shape);
	plotter->group()->addToGroup(shape);
      }
      continue;
    }
    const PlotHist1D *hist=dynamic_cast<const PlotHist1D *> (p);
    if (hist) {
      PlotHist1D::Properties properties=hist->properties();
      
      QPen pen =properties.pen;
      QBrush brush=properties.brush;
      int symbolSize=properties.symbolSize;
      QSizeF siz(symbolSize,symbolSize);
      PlotHist1D::Properties::SymbolStyle symbolStyle=properties.symbolStyle;
      bool drawSymbol=properties.plotStyle==PlotHist1D::Properties::SYMBOLS;
      QAbstractGraphicsShapeItem *shape=NULL;
      if (drawSymbol) {
	QPointF ctr(-5*charWidth,-(0.65+count++)*lineSpacing);
	if (symbolStyle==PlotHist1D::Properties::CIRCLE) {
	  shape = new QGraphicsEllipseItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotHist1D::Properties::SQUARE) {
	  shape = new QGraphicsRectItem(QRectF(MQ-ctr,siz));
	}
	else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_U) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2,0)));
	  points.push_back(MQ-ctr+(QPointF(0,symbolSize)));
	  shape = new QGraphicsPolygonItem(QPolygonF(points));
	}
	else if (symbolStyle==PlotHist1D::Properties::TRIANGLE_L) {
	  QVector<QPointF> points;
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize,   0)));
	  points.push_back(MQ-ctr+(QPointF(symbolSize/2, symbolSize)));
	  points.push_back(MQ-ctr+(QPointF(0,            0)));
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
      }
      // the histogram is drawn as a line:
      else {
	// Not filled with a brush of some type:
	if (properties.brush.style()==Qt::NoBrush) {
	  QPointF ctr(0,-(0.65+count++)*lineSpacing);
	  QPointF LP=MQ-ctr;
	  QGraphicsLineItem *line=new QGraphicsLineItem(LP.x(),LP.y(), LP.x()+11*charWidth, LP.y()); 
	  line->setPen(pen);
	  line->setMatrix(mInverse);
	  plotter->scene()->addItem(line);
	  plotter->group()->addToGroup(line);
	}
	// Filled with a brush of some type:
	else {
	  QPointF ctr(0,-(0.3+count++)*lineSpacing);
	  shape = new QGraphicsRectItem(QRectF(MQ-ctr,QSizeF(11*charWidth,0.75*lineSpacing)));
	  shape->setPen(pen);
	  shape->setBrush(brush);
	  shape->setMatrix(mInverse);
	  plotter->scene()->addItem(shape);
	  plotter->group()->addToGroup(shape);
	}
      }
      continue;
    }
    const PlotErrorEllipse *ellipse=dynamic_cast<const PlotErrorEllipse *> (p);
    if (ellipse) {
      QPointF ctr(0,-(0.65+count++)*lineSpacing);
      QPointF LP=MQ-ctr;
      PlotErrorEllipse::Properties properties=ellipse->properties();
      QPen pen =properties.pen;
      QGraphicsLineItem *line=new QGraphicsLineItem(LP.x(),LP.y(), LP.x()+11*charWidth, LP.y()); 
      line->setPen(pen);
      line->setMatrix(mInverse);
      plotter->scene()->addItem(line);
      plotter->group()->addToGroup(line);
      continue;
    }
    const PlotFunction1D *function=dynamic_cast<const PlotFunction1D *> (p);
    if (function) {
      QPointF ctr(0,-(0.65+count++)*lineSpacing);
      QPointF LP=MQ-ctr;
      PlotFunction1D::Properties properties=function->properties();
      QPen pen =properties.pen;
      QGraphicsLineItem *line=new QGraphicsLineItem(LP.x(),LP.y(), LP.x()+11*charWidth, LP.y()); 
      line->setPen(pen);
      line->setMatrix(mInverse);
      plotter->scene()->addItem(line);
      plotter->group()->addToGroup(line);
      continue;
    }
    const PlotOrbit *orbit=dynamic_cast<const PlotOrbit *> (p);
    if (orbit) {
      QPointF ctr(0,-(0.65+count++)*lineSpacing);
      QPointF LP=MQ-ctr;
      PlotOrbit::Properties properties=orbit->properties();
      QPen pen =properties.pen;
      QGraphicsLineItem *line=new QGraphicsLineItem(LP.x(),LP.y(), LP.x()+11*charWidth, LP.y()); 
      line->setPen(pen);
      line->setMatrix(mInverse);
      plotter->scene()->addItem(line);
      plotter->group()->addToGroup(line);
      continue;
    }
    std::cerr << "Error, plotable unknown to PlotKey" << std::endl;
  }
  
  delete toLogX;
  delete toLogY;
}



QFont PlotKey::font() const {
  return c->textDocument->defaultFont();
}

void PlotKey::setFont(const QFont & font) {
  return c->textDocument->setDefaultFont(font);
}


void PlotKey::add(const Plotable *plotable, const std::string & label) {
  QTextCursor cursor(c->textDocument);
  c->plotable.push_back(plotable);
  //
  // Here we add the text:
  //
  cursor.beginEditBlock();
  cursor.movePosition(QTextCursor::End);
  cursor.insertText("            "); // 12 spaces
  cursor.insertText(label.c_str());
  cursor.insertText("\n");
  cursor.endEditBlock();
}




