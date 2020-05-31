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

#include "QatPlotting/PlotText.h"
#include "QatPlotting/LinToLog.h"
#include "QatPlotting/AbsPlotter.h"
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QGraphicsRectItem>
#include <QTextDocument>
#include <iostream>
#include <iomanip>
#include <sstream>

class PlotText::Clockwork {

  public:

  Clockwork():textDocument() {}
  ~Clockwork() {
    delete textDocument;
    delete nRect;
  }


  QPointF                               point;
  QTextDocument                         *textDocument;
  QRectF                                *nRect;
};



PlotText::PlotText (const PlotText & right):Plotable(),c(new Clockwork()){
  c->point=right.c->point;
  c->textDocument=right.c->textDocument->clone();
  c->nRect=NULL;
}

PlotText & PlotText::operator=(const PlotText & right) {
  if (&right!=this) {
    c->point=right.c->point;
    c->textDocument=right.c->textDocument->clone();
    c->nRect=NULL;
  }
  return *this;
}

// Constructor
PlotText::PlotText(double x, double y, const QString & text)
  :Plotable(),c(new Clockwork())

{
  c->point=QPointF(x,y);
  c->textDocument=new QTextDocument(text);
  c->nRect=NULL;
}



// Destructor
PlotText::~PlotText(){
  delete c;
}


// Get the "natural maximum R"
const QRectF & PlotText::rectHint() const {
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
void PlotText::describeYourselfTo(AbsPlotter *plotter) const{

  LinToLog *toLogX= plotter->isLogX() ? new LinToLog (plotter->qrect()->left(),plotter->qrect()->right()) : NULL;
  LinToLog *toLogY= plotter->isLogY() ? new LinToLog (plotter->qrect()->top(),plotter->qrect()->bottom()) : NULL;

  double x=c->point.x();
  double y=c->point.y();

  QMatrix m=plotter->matrix(),mInverse=m.inverted();

  if (toLogX) x = (*toLogX)(x);
  if (toLogY) y = (*toLogY)(y);



  QGraphicsTextItem *item = new QGraphicsTextItem();
  //QPointF p=c->point;
  //QPointF p1  = m.map(c->point);
  //QPointF p2 = mInverse.map(c->point);
 

  QPointF P(plotter->qrect()->left(), plotter->qrect()->bottom());
  QPointF Q(QPointF(x,y));

  item->setDocument(c->textDocument);
  item->setPos(Q-P);
  item->setMatrix(mInverse);
  plotter->scene()->addItem(item);
  plotter->group()->addToGroup(item);
  
  delete toLogX;
  delete toLogY;
}


void PlotText::setDocument(QTextDocument *document) {
  delete c->textDocument;
  c->textDocument=document->clone();
}



QFont PlotText::font() const {
  return c->textDocument->defaultFont();
}

void PlotText::setFont(const QFont & font) {
  return c->textDocument->setDefaultFont(font);
}
