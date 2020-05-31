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

#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/LinearRangeDivider.h"
#include "QatPlotWidgets/LinearSubdivider.h"
#include "QatPlotWidgets/LogSubdivider.h"
#include "QatPlotWidgets/LogRangeDivider.h"
#include "QatPlotting/LinToLog.h"
#include "QatPlotting/Plotable.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QResizeEvent>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QClipboard>
#include <QTimer>
#include <QFileDialog>
#include <QFontDialog>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QPicture>
#include <QSvgGenerator>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <memory>
#include "QatPlotWidgets/LabelEditorDialog.h"
#include "QatPlotWidgets/PVControl.h"
#include "ui_PVControl.h"

std::ostream & operator << (std::ostream & o, const QPointF &p ) {
  return o << '(' << p.x() << ' ' << p.y() << ')'; 
}

std::ostream & operator << (std::ostream & o, const QPoint &p ) {
  return o << '(' << p.x() << ' ' << p.y() << ')'; 
}


std::ostream & operator << (std::ostream & o, const QMatrix &m ) {
  return o << '(' << m.m11() 
	   << ' ' 
	   << m.m12() 
	   << ' ' 
	   << m.m21() 
	   << ' ' 
	   << m.m22() 
	   << ' ' 
	   << m.dx() 
	   << ' ' 
	   << m.dy() 
	   << ' '  << ')'; 
}

class SRectF: public QRectF {
public:
  inline SRectF(const QPolygonF & poly) {
    if (poly.size()!=4) {
      throw std::runtime_error("Constructing a rectangle requires 4 point polygon");
    }
    else { 
      double width = poly[1].x()-poly[0].x(), height= (poly[2].y()-poly[0].y());
      setRect(poly[0].x(), poly[0].y(), width, height);
    }
  }
};


class LabelItem:public QGraphicsRectItem{
public:

  LabelItem(const QRectF & rect, QDialog *dialog, bool visible=false):
    QGraphicsRectItem(rect),dialog(dialog),visible(visible)
  {

    {
      QPen pen;
      pen.setColor(QColor("lightGray"));
      pen.setWidth(0);
      pen.setStyle(visible ? Qt::SolidLine : Qt::NoPen);
      defaultPen=pen;
    }
    {
      QPen pen;
      pen.setColor(QColor("darkBlue"));
      pen.setWidth(0);
      pen.setStyle(Qt::SolidLine);
      hoverPen=pen;
    }

    setPen(defaultPen);
    setBrush(defaultBrush);


    setAcceptHoverEvents(true); 

  }

  void hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    setPen(hoverPen);
    setBrush(hoverBrush);
  }

  void hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    setPen(defaultPen);
    setBrush(defaultBrush);
  }

  void mousePressEvent(QGraphicsSceneMouseEvent *) {
    if (dialog) {
      if (!dialog->isVisible()) {
	dialog->move(dialog->parentWidget()->pos()+dialog->parentWidget()->rect().bottomRight());
	dialog->show();
      }
      else 
	{
	  dialog->hide();
	}
    }
  }
  bool isVisible() const {return visible;}

  QPen     defaultPen, hoverPen;
  QBrush   defaultBrush, hoverBrush;
  

private:
  QDialog *dialog;
  bool     visible;
};

class GraphBoxItem:public QGraphicsRectItem{
public:

  GraphBoxItem(const QRectF & rect, PVControl *control, PlotView *parent=NULL):
    QGraphicsRectItem(rect),popup(new QMenu(parent)), control(control),pvParent(parent)
  {
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("black"));
    setPen(pen);
    setAcceptHoverEvents(true); 

    QSize size(150,75);
    popup->resize(size);
    copyAction=popup->addAction("Copy");
    saveAction=popup->addAction("Save");
    printAction=popup->addAction("Print");

    QObject::connect (printAction, SIGNAL(triggered()), parent, SLOT(print()));
    QObject::connect (copyAction, SIGNAL(triggered()),  parent, SLOT(copy()));
    QObject::connect (saveAction, SIGNAL(triggered()),  parent, SLOT(save()));
    
  }

  void hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("darkBlue"));
    setPen(pen);
  }

  void hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("black"));
    setPen(pen);
  }

  void mousePressEvent (QGraphicsSceneMouseEvent *e) {
    Qt::KeyboardModifiers m=e->modifiers();
    bool isShift = m & 0x02000000;

    //
    // You have no middle button?  Then right button +shift
    //

    if (e->buttons()==Qt::RightButton) {
      popup->popup(e->widget()->mapToGlobal(e->pos().toPoint()));
    }
    else if (e->buttons()==Qt::MidButton
	     || ((e->buttons()==Qt::LeftButton) && isShift)
	     ) {
      double slX = (pvParent->qrect()->right() - pvParent->qrect()->left())/double(rect().width());
      double slY = (pvParent->qrect()->top() - pvParent->qrect()->bottom())/double(rect().height());
      double x = (e->pos().toPoint().x()-rect().left())   * slX + pvParent->qrect()->left();
      double y = (e->pos().toPoint().y()-rect().bottom()) * slY + pvParent->qrect()->top();
      pvParent->pointChosen(x,y);
    }
    else if (e->buttons()==Qt::LeftButton && !isShift) {
      control->move(control->parentWidget()->pos()+control->parentWidget()->rect().topRight());
      if (!control->isVisible()) {
	control->show();
	control->raise();
      }
      else {
	control->hide();
      }
    }
  }

  void mouseReleaseEvent (QGraphicsSceneMouseEvent *e) {
    if (e->buttons()==Qt::RightButton) popup->hide();
  }


private:
  QMenu     *popup;
  PVControl *control;
  QAction   *printAction;
  QAction   *copyAction;
  QAction   *saveAction;
  PlotView  *pvParent;
};

 
class PlotView::Clockwork {


public:


  Clockwork():
    created(false),
    control(NULL),
    graphBox(NULL),
    xLabelBox(NULL),             yLabelBox(NULL),             vLabelBox(NULL),             titleBox(NULL),           statBox(NULL),
    xLabelEditorDialog(NULL),    yLabelEditorDialog(NULL),    vLabelEditorDialog(NULL),    titleEditorDialog(NULL),  statEditorDialog(NULL),
    xLabelTextItem(NULL),        yLabelTextItem(NULL),        vLabelTextItem(NULL),        titleTextItem(NULL),      statTextItem(NULL),
    g(NULL),gridLinesGroup(NULL),contentsGroup(NULL),xZeroLine(NULL),yZeroLine(NULL),
    xRangeDivider(NULL),yRangeDivider(NULL), xAllocDivider(NULL), yAllocDivider(NULL),
    labelWidth(50)
  {
      xAxisFont.setPointSize(18);
      xAxisFont.setFamily("Arial");
      yAxisFont.setPointSize(18);
      yAxisFont.setFamily("Arial");
      xPerCento=40;
      yPerCento=40;
  }

  struct TickLabel{
    TickLabel(double pos, QGraphicsTextItem*tI) :pos(pos),tI(tI) {}
    double pos;
    QGraphicsTextItem *tI;
  };
  
  void resizeBoxes(int w, int h) {
    graphBoxTopLeft=QPoint(2*labelWidth, 2*labelWidth);
    graphBoxSize=QSize(w-4*labelWidth, h-4*labelWidth);
    titleBoxTopLeft=QPoint(2*labelWidth,    labelWidth);
    titleBoxSize=QSize(w-4*labelWidth, labelWidth);
    xLabelBoxTopLeft=QPoint(2*labelWidth,   h-labelWidth);
    xLabelBoxSize=QSize(w-4*labelWidth, labelWidth);
    yLabelBoxTopLeft=QPoint(0,                2*labelWidth);
    yLabelBoxSize=QSize(labelWidth, h-4*labelWidth);
    vLabelBoxTopLeft=QPoint(w-labelWidth,     2*labelWidth);
    vLabelBoxSize=QSize(labelWidth, h-4*labelWidth);
    statBoxTopLeft=QPoint(2*labelWidth+(100-xPerCento)*(w-4*labelWidth)/100, 2*labelWidth);
    statBoxSize=QSize(xPerCento*(w-4*labelWidth)/100, yPerCento*(h-4*labelWidth)/100);
  }

  bool                created;
  PVControl           *control;
  GraphBoxItem        *graphBox;
  LabelItem           *xLabelBox,          *yLabelBox,          *vLabelBox,          *titleBox,          *statBox;
  LabelEditorDialog   *xLabelEditorDialog, *yLabelEditorDialog, *vLabelEditorDialog, *titleEditorDialog, *statEditorDialog;
  QGraphicsTextItem   *xLabelTextItem,     *yLabelTextItem,     *vLabelTextItem,     *titleTextItem,     *statTextItem;
  QGraphicsItemGroup  *g, *gridLinesGroup, *contentsGroup;
  QGraphicsLineItem   *xZeroLine, *yZeroLine;
  std::vector<TickLabel> xTickLabel, yTickLabel;
  AbsRangeDivider     *xRangeDivider, *yRangeDivider;  // These are not allocated and not owned.
  AbsRangeDivider     *xAllocDivider, *yAllocDivider;  // These area allocated and owned. 

  Style          xAxisStyle;
  Style          yAxisStyle;

  PRectF         rect;
  QRectF         qrect;
  QGraphicsScene scene;
  QFont          xAxisFont;
  QFont          yAxisFont;
  const int      labelWidth;

  QPoint graphBoxTopLeft;    
  QSize  graphBoxSize;       
  QPoint titleBoxTopLeft;    
  QSize  titleBoxSize;       
  QPoint xLabelBoxTopLeft;   
  QSize  xLabelBoxSize;      
  QPoint yLabelBoxTopLeft;   
  QSize  yLabelBoxSize;      
  QPoint vLabelBoxTopLeft;   
  QSize  vLabelBoxSize;      
  QPoint statBoxTopLeft;    
  QSize  statBoxSize;       

  QMenu *popupMenu;
  QTimer *timer;
  std::vector<Plotable *> plotableList;
  double xPerCento, yPerCento;

};


PlotView::~PlotView() {
  delete c->xAllocDivider;
  delete c->yAllocDivider;
  delete c;
}


PlotView::PlotView( QWidget *parent) :
  QGraphicsView(parent),c(new Clockwork()){
  c->rect=QRectF(0,0,1,1);
  c->qrect=QRectF(0,0,1,1);
  create();
  setLogX(false);
  setLogY(false);
  createXRangeDivider();
  createYRangeDivider();
  setFrameShape(QFrame::NoFrame);
}

PlotView::PlotView(PRectF  rect, Style xStyle, Style yStyle, bool createRangeDividers, QWidget *parent) : QGraphicsView(parent),c(new Clockwork)
{
  c->rect=rect;
  c->qrect=rect;
  create();
  setLogX(xStyle==LOG);
  setLogY(yStyle==LOG);
  if (createRangeDividers) {
    createXRangeDivider();
    createYRangeDivider();
  }
  setFrameShape(QFrame::NoFrame);
}

void PlotView::create() {
  // If created, you have to do a big cleanup.
  if (c->created) {

    delete c->g;
    c->xZeroLine=NULL; // Now gone.
    c->yZeroLine=NULL; // Now gone.
    
    // Rebuild group and gridLinesGroup:
    c->g=new QGraphicsItemGroup();
    c->scene.addItem(c->g);

    c->contentsGroup=new QGraphicsItemGroup();
    c->scene.addItem(c->contentsGroup);
    c->g->addToGroup(c->contentsGroup);

    c->gridLinesGroup=new QGraphicsItemGroup();
    c->scene.addItem(c->gridLinesGroup);
    c->g->addToGroup(c->gridLinesGroup);
    

    for (unsigned int i=0;i<c->xTickLabel.size();i++) {
      c->scene.removeItem(c->xTickLabel[i].tI);
      delete c->xTickLabel[i].tI;
			  
    }
    for (unsigned int i=0;i<c->yTickLabel.size();i++) {
      c->scene.removeItem(c->yTickLabel[i].tI);
      delete c->yTickLabel[i].tI;
    }
    c->xTickLabel.erase(c->xTickLabel.begin(),c->xTickLabel.end());
    c->yTickLabel.erase(c->yTickLabel.begin(),c->yTickLabel.end());

    int w=width(),h=height();
    c->resizeBoxes(w,h);

  }
  else {
    c->created=true;

    c->timer = new QTimer(this);
    QObject::connect(c->timer, SIGNAL(timeout()), this, SLOT(animate()), Qt::QueuedConnection);

    c->control=new PVControl(this,c->qrect);
    setScene(&c->scene);
    c->g=new QGraphicsItemGroup();
    c->scene.addItem(c->g);
    c->gridLinesGroup=new QGraphicsItemGroup();
    c->scene.addItem(c->gridLinesGroup);
    c->g->addToGroup(c->gridLinesGroup);

    c->contentsGroup=new QGraphicsItemGroup();
    c->scene.addItem(c->contentsGroup);
    c->g->addToGroup(c->contentsGroup);


    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setMinimumSize(700,500);
    c->xAxisStyle=c->control->xAxisStyle();
    c->yAxisStyle=c->control->yAxisStyle();

    c->xLabelEditorDialog = new LabelEditorDialog(this); c->xLabelEditorDialog->textEdit()->setAlignment(Qt::AlignHCenter);
    c->yLabelEditorDialog = new LabelEditorDialog(this); c->yLabelEditorDialog->textEdit()->setAlignment(Qt::AlignHCenter);
    c->vLabelEditorDialog = new LabelEditorDialog(this); c->vLabelEditorDialog->textEdit()->setAlignment(Qt::AlignHCenter);
    c->titleEditorDialog  = new LabelEditorDialog(this); c->titleEditorDialog->textEdit()->setAlignment(Qt::AlignHCenter);
    c->statEditorDialog   = new LabelEditorDialog(this); 

    QSize    screenDimensions(700,500);

    setDragMode(NoDrag);
    //    setRenderHint(QPainter::Antialiasing);
    // setCacheMode(QGraphicsView::CacheBackground);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SRectF sceneRect=mapToScene(0,0,700,500);

    setSceneRect(sceneRect);
    resize(screenDimensions.width(),screenDimensions.height());
    
    int w=screenDimensions.width()/*-5*/,h=screenDimensions.height()/*-5*/;
    c->resizeBoxes(w,h);

    c->statBox=new LabelItem(QRect(c->statBoxTopLeft,c->statBoxSize),c->statEditorDialog); 
    {
      QPen pen;
      pen.setStyle(Qt::NoPen);
      c->statBox->defaultPen=pen;
    }
    c->scene.addItem(c->statBox);

    c->graphBox=new GraphBoxItem(QRect(c->graphBoxTopLeft,c->graphBoxSize),c->control, this);
    c->scene.addItem(c->graphBox);
    c->titleBox=new LabelItem(QRect(c->titleBoxTopLeft,c->titleBoxSize),c->titleEditorDialog); 
    c->scene.addItem(c->titleBox);
    c->xLabelBox=new LabelItem(QRect(c->xLabelBoxTopLeft,c->xLabelBoxSize),c->xLabelEditorDialog);    
    c->scene.addItem(c->xLabelBox);
    c->yLabelBox=new LabelItem(QRect(c->yLabelBoxTopLeft,c->yLabelBoxSize),c->yLabelEditorDialog);
    c->scene.addItem(c->yLabelBox);
    c->vLabelBox=new LabelItem(QRect(c->vLabelBoxTopLeft,c->vLabelBoxSize),c->vLabelEditorDialog);
    c->scene.addItem(c->vLabelBox);

    //Make the stat box movable; set boundary to zero.
    c->statBox->setFlag(QGraphicsItem::ItemIsMovable);
    {
      QPen pen;
      pen.setStyle(Qt::NoPen);
      c->statBox->defaultPen=pen;
    }
    
    // Workaround to Qt Bug # 138596 (Qt 4.2.1)
    c->titleEditorDialog->textEdit()->document()->setTextWidth(c->titleBoxSize.width());
    c->xLabelEditorDialog->textEdit()->document()->setTextWidth(c->xLabelBoxSize.width());
    c->yLabelEditorDialog->textEdit()->document()->setTextWidth(c->yLabelBoxSize.width());
    c->vLabelEditorDialog->textEdit()->document()->setTextWidth(c->vLabelBoxSize.width());
    c->statEditorDialog->textEdit()->document()->setTextWidth(c->statBoxSize.width());


    c->titleEditorDialog->textEdit()->setFixedWidth((int) c->titleBox->rect().width());
    c->titleEditorDialog->setFixedWidth(c->titleEditorDialog->childrenRect().width());
    
    c->xLabelEditorDialog->textEdit()->setFixedWidth((int) c->xLabelBox->rect().width());
    c->xLabelEditorDialog->setFixedWidth(c->xLabelEditorDialog->childrenRect().width());
    
    c->yLabelEditorDialog->textEdit()->setFixedWidth((int) c->yLabelBox->rect().height());
    c->yLabelEditorDialog->setFixedWidth(c->yLabelEditorDialog->childrenRect().width());
    
    c->vLabelEditorDialog->textEdit()->setFixedWidth((int) c->vLabelBox->rect().height());
    c->vLabelEditorDialog->setFixedWidth(c->vLabelEditorDialog->childrenRect().width());
    
    c->statEditorDialog->textEdit()->setFixedWidth((int) c->statBox->rect().width());
    c->statEditorDialog->textEdit()->setFixedHeight((int) c->statBox->rect().height());
    // end workaround


    c->statEditorDialog->textEdit()->document()->setTextWidth(c->statBoxSize.width());
    c->statEditorDialog->textEdit()->setWordWrapMode(QTextOption::WrapAnywhere);
    c->statEditorDialog->textEdit()->setLineWrapMode(QTextEdit::FixedPixelWidth);
    
    
    QFont font;
    font.setPointSize(24);
    font.setFamily("Arial");
    
    
    c->titleTextItem = new QGraphicsTextItem(c->titleBox);
    c->titleTextItem->setFont(font);
    //c->scene.addItem(c->titleTextItem);
    
    c->xLabelTextItem = new QGraphicsTextItem(c->xLabelBox);
    c->xLabelTextItem->setFont(font);
    //c->scene.addItem(c->xLabelTextItem);
    
    c->yLabelTextItem = new QGraphicsTextItem(c->yLabelBox);
    c->yLabelTextItem->setFont(font);
    //c->scene.addItem(c->yLabelTextItem);
    
    c->vLabelTextItem = new QGraphicsTextItem(c->vLabelBox);
    c->vLabelTextItem->setFont(font);
    //c->scene.addItem(c->vLabelTextItem);
    
    c->statTextItem = new QGraphicsTextItem(c->statBox);
    c->statTextItem->setFont(font);
    //c->scene.addItem(c->statTextItem);
    c->statBox->setFlag(QGraphicsItem::ItemIsSelectable);
    
    connect(c->control, SIGNAL(rectChanged(const QRectF &)), this, SLOT(setRect (const QRectF &)));
    connect(c->control, SIGNAL(rectChanged(const QRectF &)), this, SLOT(setRect (const QRectF &)));
    
    connect(c->titleEditorDialog->textEdit(),SIGNAL(textChanged()),this, SLOT(updateTitle()));
    connect(c->xLabelEditorDialog->textEdit(),SIGNAL(textChanged()),this, SLOT(updateXLabel()));
    connect(c->yLabelEditorDialog->textEdit(),SIGNAL(textChanged()),this, SLOT(updateYLabel()));
    connect(c->vLabelEditorDialog->textEdit(),SIGNAL(textChanged()),this, SLOT(updateVLabel()));
    connect(c->statEditorDialog->textEdit(),SIGNAL(textChanged()),this, SLOT(updateStat()));
   
    connect(c->control, SIGNAL(labelHome()), this, SLOT(labelHome()));
    connect(c->control, SIGNAL(editXAxisFont()), this, SLOT(editXAxisFont()));
    connect(c->control, SIGNAL(editYAxisFont()), this, SLOT(editYAxisFont()));

    connect(c->control, SIGNAL(labelXSizePercentageChanged(qreal)), this, SLOT(setLabelXSizePercentage(qreal)));
    connect(c->control, SIGNAL(labelYSizePercentageChanged(qreal)), this, SLOT(setLabelYSizePercentage(qreal)));
    c->popupMenu=new QMenu(this);
    
  }


  // Label the x axes:
  if (c->xRangeDivider) c->xRangeDivider->setRange(c->qrect.left(), c->qrect.right());
  if (c->yRangeDivider) c->yRangeDivider->setRange(c->qrect.top(), c->qrect.bottom());
 
  LinToLog *toLogY= c->yAxisStyle==LOG ? new LinToLog (c->qrect.top(),c->qrect.bottom()) : NULL;
  LinToLog *toLogX= c->xAxisStyle==LOG ? new LinToLog (c->qrect.left(),c->qrect.right()) : NULL;
  // Master transformer:
  QMatrix m;
  m.scale(c->graphBoxSize.width()/c->qrect.width(),-c->graphBoxSize.height()/c->qrect.height());
  m.translate(-c->qrect.bottomLeft().x(),  -c->qrect.bottomLeft().y());
  // End master transformer..

  if (c->xRangeDivider) {
    for (int ix=0;ix<c->xRangeDivider->getNumSubdivisions();ix++) {
      if (!c->xRangeDivider->isValid(ix)) continue;

      std::ostringstream x_label;
      double xValue=c->xRangeDivider->getSubdivision(ix).x();
      double yValue=c->qrect.top();
      double wValue=c->qrect.bottom();
      if (toLogX) xValue = (*toLogX)(xValue);
      
      

      QPointF X (xValue,yValue);
      QPointF XP(xValue,yValue+c->qrect.height()/32.0);
      QGraphicsLineItem *line=new QGraphicsLineItem();
      line->setLine(QLineF(X,XP));
      QPen pen;
      pen.setWidth(0);
      line->setPen(pen);
      c->scene.addItem(line);
      c->g->addToGroup(line);
      // Again (along the top).
      {      
	QPointF X (xValue,wValue);
	QPointF XP(xValue,wValue-c->qrect.height()/32.0);
	QGraphicsLineItem *line=new QGraphicsLineItem();
	line->setLine(QLineF(X,XP));
	QPen pen;
	pen.setWidth(0);
	line->setPen(pen);
	c->scene.addItem(line);
	c->g->addToGroup(line);
      }      
      // More (subdivisions, linear:):
      if (!isLogX()) {
	int nPass=ix==0 ? 2:1;
	for (int n=0;n<nPass;n++) {
	  std::shared_ptr<AbsRangeDivider> subdivider(new LinearSubdivider());
	  double xValue=c->xRangeDivider->getSubdivision(ix).x();
	  double xPlusValue= ix!=c->xRangeDivider->getNumSubdivisions()-1 ? c->xRangeDivider->getSubdivision(ix+1).x(): 2*xValue-c->xRangeDivider->getSubdivision(ix-1).x();
	    // Works for linear. 

	  // This is a second pass:
	  if (n==1) {
	    if (ix==0) {
	      double interval=xPlusValue-xValue;
	      xPlusValue=xValue;
	      xValue = xValue-interval;
	    }
	  }

	  subdivider->setRange(xValue,xPlusValue);
	  for (int i=0;i<subdivider->getNumSubdivisions();i++) {
	    //	    if (!subdivider->isValid(i)) continue;
	    double xValue=subdivider->getSubdivision(i).x(); // Shadows!!
	    if (xValue<c->qrect.left() || xValue>c->qrect.right()) continue;
	    {
	      QPointF X (xValue,yValue);
	      QPointF XP(xValue,yValue+c->qrect.height()/64.0);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	    {
	      QPointF X (xValue,wValue);
	      QPointF XP(xValue,wValue-c->qrect.height()/64.0);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	  }
	}
      }
      else { // Log X
	int nPass=ix==0 || ix == c->xRangeDivider->getNumSubdivisions()-1? 2:1;
	for (int n=0;n<nPass;n++) {
	  std::shared_ptr<AbsRangeDivider> subdivider(new LogSubdivider());
	  double xValue=c->xRangeDivider->getSubdivision(ix).x();
	  double xPlusValue= ix!=c->xRangeDivider->getNumSubdivisions()-1 ? c->xRangeDivider->getSubdivision(ix+1).x(): 2*xValue-c->xRangeDivider->getSubdivision(ix-1).x();
	  if (n==1) {
	    if (ix==0) {
	      xPlusValue=xValue;
	      xValue=c->qrect.left();
	    }
	    else {
	      xValue=xPlusValue;
	      xPlusValue=c->qrect.right();
	    }
	  }

	  subdivider->setRange(xValue,xPlusValue);
	  for (int i=0;i<subdivider->getNumSubdivisions();i++) {
	    double xValue=subdivider->getSubdivision(i).x(); // Shadows!!
	    xValue = (*toLogX)(xValue);
	    if (xValue<c->qrect.left() || xValue>c->qrect.right()) continue;
	    {
	      QPointF X (xValue,yValue);
	      QPointF XP(xValue,yValue+c->qrect.height()/64.0);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	    {
	      QPointF X (xValue,wValue);
	      QPointF XP(xValue,wValue-c->qrect.height()/64.0);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	  }
	}
      }

      QGraphicsTextItem *anon = new QGraphicsTextItem();
      anon->setDocument(c->xRangeDivider->getSubdivision(ix).label());
      anon->setFont(c->xAxisFont);
      anon->adjustSize();
      c->scene.addItem(anon);
      QRect SBR=anon->sceneBoundingRect().toRect();
      anon->setPos(m.map(X)+c->graphBoxTopLeft-QPoint(SBR.width()/2,0));
      c->xTickLabel.push_back(Clockwork::TickLabel(xValue,anon));
      
      {
	QGraphicsLineItem *line=new QGraphicsLineItem(QLineF(QPointF(xValue,c->qrect.top()),QPointF(xValue,c->qrect.bottom())));
	QPen  pen=line->pen();
	pen.setStyle(Qt::DotLine);
	pen.setColor(QColor("lightGray"));
	pen.setWidth(0);
	line->setPen(pen);
	c->scene.addItem(line);
	c->gridLinesGroup->addToGroup(line);
      }
    }
  }
  if (c->yRangeDivider) {
    for (int iy=0;iy<c->yRangeDivider->getNumSubdivisions();iy++) {
      if (!c->yRangeDivider->isValid(iy)) continue;

      std::ostringstream y_label;
      double yValue = c->yRangeDivider->getSubdivision(iy).x();
      double xValue = c->qrect.left();
      double wValue = c->qrect.right();
      if (toLogY) yValue = (*toLogY)(yValue);
       
      
      QPointF  X(xValue,yValue);
      QPointF  XP(xValue+c->qrect.width()/32.0,yValue);
      QGraphicsLineItem *line=new QGraphicsLineItem();
      line->setLine(QLineF(X,XP));
      QPen pen;
      pen.setWidth(0);
      line->setPen(pen);
      c->scene.addItem(line);
      c->g->addToGroup(line);
      // Again:
      {
	QPointF  X(wValue,yValue);
	QPointF  XP(wValue-c->qrect.width()/32.0,yValue);
	QGraphicsLineItem *line=new QGraphicsLineItem();
	line->setLine(QLineF(X,XP));
	QPen pen;
	pen.setWidth(0);
	line->setPen(pen);
	c->scene.addItem(line);
	c->g->addToGroup(line);
      }
      // More (subdivisions, linear:):
      if (!isLogY()) {
	int nPass=iy==0 ? 2:1;
	for (int n=0;n<nPass;n++) {
	  std::shared_ptr<AbsRangeDivider> subdivider(new LinearSubdivider());
	  double yValue=c->yRangeDivider->getSubdivision(iy).x();
	  double yPlusValue= iy!=c->yRangeDivider->getNumSubdivisions()-1 ? c->yRangeDivider->getSubdivision(iy+1).x(): 2*yValue-c->yRangeDivider->getSubdivision(iy-1).x();
	    // Works for linear. 

	  // This is a second pass:
	  if (n==1) {
	    if (iy==0) {
	      double interval=yPlusValue-yValue;
	      yPlusValue=yValue;
	      yValue = yValue-interval;
	    }
	  }

	  subdivider->setRange(yValue,yPlusValue);
	  for (int i=0;i<subdivider->getNumSubdivisions();i++) {
	    //	    if (!subdivider->isValid(i)) continue;
	    double yValue=subdivider->getSubdivision(i).x(); // Shadows!!
	    if (yValue<c->qrect.top() || yValue>c->qrect.bottom()) continue;
	    {
	      QPointF X (xValue,yValue);
	      QPointF XP(xValue+c->qrect.width()/64.0,yValue);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	    {
	      QPointF X (wValue,yValue);
	      QPointF XP(wValue-c->qrect.width()/64.0,yValue);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	  }
	}
      }
      else { // Log Y
	int nPass=iy==0 || iy == c->yRangeDivider->getNumSubdivisions()-1? 2:1;
	for (int n=0;n<nPass;n++) {
	  std::shared_ptr<AbsRangeDivider> subdivider(new LogSubdivider());
	  double yValue=c->yRangeDivider->getSubdivision(iy).x();
	  double yPlusValue= iy!=c->yRangeDivider->getNumSubdivisions()-1 ? c->yRangeDivider->getSubdivision(iy+1).x(): 2*yValue-c->yRangeDivider->getSubdivision(iy-1).x();
	  if (n==1) {
	    if (iy==0) {
	      yPlusValue=yValue;
	      yValue=c->qrect.top();
	    }
	    else {
	      yValue=yPlusValue;
	      yPlusValue=c->qrect.bottom();
	    }
	  }

	  subdivider->setRange(yValue,yPlusValue);
	  for (int i=0;i<subdivider->getNumSubdivisions();i++) {
	    double yValue=subdivider->getSubdivision(i).x(); // Shadows!!
	    yValue = (*toLogY)(yValue);
	    if (yValue<c->qrect.top() || yValue>c->qrect.bottom()) continue;
	    {
	      QPointF X (xValue,yValue);
	      QPointF XP(xValue+c->qrect.width()/64.0,yValue);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	    {
	      QPointF X (wValue,yValue);
	      QPointF XP(wValue-c->qrect.width()/64.0,yValue);
	      QGraphicsLineItem *line=new QGraphicsLineItem();
	      line->setLine(QLineF(X,XP));
	      QPen pen;
	      pen.setWidth(0);
	      line->setPen(pen);
	      c->scene.addItem(line);
	      c->g->addToGroup(line);
	    }
	  }
	}
      }
      
      QGraphicsTextItem *anon = new QGraphicsTextItem();
      anon->setDocument(c->yRangeDivider->getSubdivision(iy).label());
      anon->setFont(c->yAxisFont);
      anon->adjustSize();
      c->scene.addItem(anon);
      QRect SBR=anon->sceneBoundingRect().toRect();
      anon->setPos(m.map(X).toPoint()+c->graphBoxTopLeft-QPoint(2*SBR.width()/2+10,SBR.height()/2));
      c->yTickLabel.push_back(Clockwork::TickLabel(yValue,anon));
      
      {
	QGraphicsLineItem *line=new QGraphicsLineItem(QLineF(QPointF(c->qrect.left(),yValue),QPointF(c->qrect.right(),yValue)));
	QPen  pen=line->pen();
	pen.setStyle(Qt::DotLine);
	pen.setColor(QColor("lightGray"));
	pen.setWidth(0);
	line->setPen(pen);
	c->scene.addItem(line);
	c->gridLinesGroup->addToGroup(line);
      }
      
    }
  }
  if (c->qrect.contains(0,c->qrect.center().y())) 
  {
    c->xZeroLine=new QGraphicsLineItem(QLineF(QPointF(0,c->qrect.bottom()),QPointF(0,c->qrect.top())));
    QPen  pen=c->xZeroLine->pen();
    pen.setColor(QColor("lightGray"));
    pen.setWidth(0);
    c->xZeroLine->setPen(pen);
    c->scene.addItem(c->xZeroLine);
    c->g->addToGroup(c->xZeroLine);
  }

  if (c->qrect.contains(c->qrect.center().x(), 0)) 
  {
    c->yZeroLine=new QGraphicsLineItem(QLineF(QPointF(c->qrect.right(),0),QPointF(c->qrect.left(),0)));
    QPen  pen=c->yZeroLine->pen();
    pen.setColor(QColor("lightGray"));
    pen.setWidth(0);
    c->yZeroLine->setPen(pen);
    c->scene.addItem(c->yZeroLine);
    c->g->addToGroup(c->yZeroLine);
  }

  //=============================HERE IS A PLACE TO PLAY!!!
  // 
  // Scene
  // ContentsGroup
  // Rect
  // Matrix and inverse

  for (unsigned int i=0;i<c->plotableList.size();i++) {
    c->plotableList[i]->describeYourselfTo(this);
  }
  //=============================END OF PLAYGROUND

  // Now go through the contents group and order everything!
  QList<QGraphicsItem *> contentsList =c->contentsGroup->childItems();
  QList<QGraphicsItem *> gridLinesList=c->gridLinesGroup->childItems();
  QList<QGraphicsItem *> gList        =c->g->childItems();


  for (int i=0;i<contentsList.size();i++) {
    contentsList.at(i)->setZValue(i);
  }
  for (int i=0;i<gridLinesList.size();i++) {
    gridLinesList.at(i)->setZValue(i);
  }
  for (int i=0;i<gList.size();i++) {
    gList.at(i)->setZValue(i);
  }
  c->graphBox->setZValue(0);
  c->g->setZValue(1);
  c->vLabelBox->setZValue(2);
  c->vLabelTextItem->setZValue(3);
  c->titleBox->setZValue(4);
  c->titleTextItem->setZValue(5);
  c->xLabelBox->setZValue(6);
  c->xLabelTextItem->setZValue(7);
  c->yLabelBox->setZValue(8);
  c->yLabelTextItem->setZValue(9);
  c->statBox->setZValue(10);    
  c->statTextItem->setZValue(11);    

  delete toLogY;

  c->g->setMatrix(m);
  c->g->setPos(c->graphBoxTopLeft);   

  setGrid(c->control->ui().gridCheckBox->isChecked());
  setXZero(c->control->ui().xZeroCheckBox->isChecked());
  setYZero(c->control->ui().yZeroCheckBox->isChecked());

  
}


void PlotView::resizeEvent (QResizeEvent */*event*/) {
  // This is here to prevent funny sizes appearing during resize:
  create();

  //int h=size.height();
  //int w=size.width();

  int h=height();
  int w=width();

  c->resizeBoxes(w,h);


  QMatrix m;
  m.scale(c->graphBoxSize.width()/c->qrect.width(),-c->graphBoxSize.height()/c->qrect.height());
  m.translate(-c->qrect.bottomLeft().x(),  -c->qrect.bottomLeft().y());
  
  c->g->setMatrix(m);
  c->g->setPos(c->graphBoxTopLeft);

  c->graphBox->setRect(QRect(c->graphBoxTopLeft,c->graphBoxSize));

  c->titleBox ->setRect(QRect(c->titleBoxTopLeft,c->titleBoxSize)); 
  c->titleTextItem->setPos(c->titleBox->sceneBoundingRect().topLeft());

  c->xLabelBox->setRect(QRect(c->xLabelBoxTopLeft,c->xLabelBoxSize));    
  c->xLabelTextItem->setPos(c->xLabelBox->sceneBoundingRect().topLeft());

  c->yLabelBox->setRect(QRect(c->yLabelBoxTopLeft,c->yLabelBoxSize));
  c->yLabelTextItem->setPos(c->yLabelBox->sceneBoundingRect().bottomLeft());

  c->vLabelBox->setRect(QRect(c->vLabelBoxTopLeft,c->vLabelBoxSize));
  c->vLabelTextItem->setPos(c->vLabelBox->sceneBoundingRect().bottomLeft());

  c->statBox ->setRect(QRect(c->statBoxTopLeft,c->statBoxSize)); 
  c->statTextItem->setPos(c->statBox->sceneBoundingRect().topLeft()-c->statBox->pos());

  for (unsigned int i=0;i<c->yTickLabel.size();i++) {
    QGraphicsTextItem *anon=c->yTickLabel[i].tI;
    double yValue=c->yTickLabel[i].pos;
    double xValue=c->qrect.left();
    QPointF X(xValue,yValue);
    QRect SBR=anon->sceneBoundingRect().toRect();
    anon->setPos( m.map(X).toPoint()+c->graphBoxTopLeft-QPoint(2*SBR.width()/2+10,SBR.height()/2));
  }

  for (unsigned int i=0;i<c->xTickLabel.size();i++) {
    QGraphicsTextItem *anon=c->xTickLabel[i].tI;
    double xValue=c->xTickLabel[i].pos;
    double yValue=c->qrect.top();
    QPointF X(xValue,yValue);
    QRect SBR=anon->sceneBoundingRect().toRect();
    anon->setPos(m.map(X)+c->graphBoxTopLeft-QPoint(SBR.width()/2,0));
  }

  // Workaround to Qt Bug # 138596 (Qt 4.2.1)
  if (1) {
    c->titleTextItem->setTextWidth(c->titleBox->rect().width());
    c->xLabelTextItem->setTextWidth(c->xLabelBox->rect().width());
    c->yLabelTextItem->setTextWidth(c->yLabelBox->rect().height());
    c->vLabelTextItem->setTextWidth(c->vLabelBox->rect().height());
    c->statTextItem->setTextWidth(c->statBox->rect().width());
    
    
    c->titleEditorDialog->textEdit()->setFixedWidth((int) c->titleBox->rect().width());
    c->titleEditorDialog->setFixedWidth(c->titleEditorDialog->childrenRect().width());
    
    c->xLabelEditorDialog->textEdit()->setFixedWidth((int) c->xLabelBox->rect().width());
    c->xLabelEditorDialog->setFixedWidth(c->xLabelEditorDialog->childrenRect().width());
    
    c->yLabelEditorDialog->textEdit()->setFixedWidth((int) c->yLabelBox->rect().height());
    c->yLabelEditorDialog->setFixedWidth(c->yLabelEditorDialog->childrenRect().width());
    
    c->vLabelEditorDialog->textEdit()->setFixedWidth((int) c->vLabelBox->rect().height());
    c->vLabelEditorDialog->setFixedWidth(c->vLabelEditorDialog->childrenRect().width());
    
    c->statEditorDialog->textEdit()->setFixedWidth((int) c->statBox->rect().width());
    c->statEditorDialog->textEdit()->setFixedHeight((int) c->statBox->rect().height());
  }

  setGrid(c->control->ui().gridCheckBox->isChecked());
  setXZero(c->control->ui().xZeroCheckBox->isChecked());
  setYZero(c->control->ui().yZeroCheckBox->isChecked());
}

// Draw Primitives into the plotter:
// Set the Plot Rectangle

void PlotView::setRect(PRectF R) {
  setRect(QRectF(R));
}
void PlotView::setRect(QRectF R){
  if (R.top()<=0) {
    if (isLogY()) throw std::runtime_error("Ymin<=0 requested with logarithmic scale");
  }
  if (R.left()<=0) {
    if (isLogX()) throw std::runtime_error("Xmin<=0 requested with logarithmic scale");
  }
  c->rect=R;
  c->qrect=R;
  create();
  resize(QWidget::size());
  c->control->blockSignals(true);
  c->control->setRect(R);
  c->control->blockSignals(false);
}


// Get the Plot Rectangle:
PRectF * PlotView::rect() {
  return &c->rect;
}

const PRectF * PlotView::rect() const {
  return &c->rect;
}

// Get the Plot Rectangle:
QRectF * PlotView::qrect() {
  return &c->qrect;
}

const QRectF * PlotView::qrect() const {
  return &c->qrect;
}

// Clear the Plots
void PlotView::clear() {
  stopAnimating();
  c->plotableList.erase(c->plotableList.begin(),c->plotableList.end());
  create();
}


void PlotView::setGrid(bool flag){
  if (flag) c->gridLinesGroup->show(); else c->gridLinesGroup->hide();
  c->scene.update(c->scene.sceneRect());
  c->control->blockSignals(true);
  c->control->ui().gridCheckBox->setChecked(flag);
  c->control->blockSignals(false);

}
void PlotView::setXZero(bool flag){
  if (c->xZeroLine) {
    if (flag) c->xZeroLine->show(); else c->xZeroLine->hide();
    c->scene.update(c->scene.sceneRect());
  }
  c->control->blockSignals(true);
  c->control->ui().xZeroCheckBox->setChecked(flag);
  c->control->blockSignals(false);
}
void PlotView::setYZero(bool flag){
  if (c->yZeroLine) {
    if (flag) c->yZeroLine->show(); else c->yZeroLine->hide();
    c->scene.update(c->scene.sceneRect());
  }
  c->control->blockSignals(true);
  c->control->ui().yZeroCheckBox->setChecked(flag);
  c->control->blockSignals(false);
}
void PlotView::setLogX(bool flag){
  if (flag==LOG && c->qrect.left()<=0) {
    throw std::runtime_error("Logarithmic scale requested where xmin<=0");
  }
  c->xAxisStyle=flag ? LOG:LINEAR;

  if (c->xRangeDivider && c->xRangeDivider==c->xAllocDivider) createXRangeDivider();
  c->control->blockSignals(true);
  c->control->ui().logXCheckBox->setChecked(flag);
  c->control->blockSignals(false);
  create();
}

void PlotView::setLogY(bool flag){
  if (flag==LOG && c->qrect.top()<=0) {
    throw std::runtime_error("Logarithmic scale requested where ymin<=0");
  }
  c->yAxisStyle=flag ? LOG:LINEAR;
  if (c->yRangeDivider && c->yRangeDivider==c->yAllocDivider) createYRangeDivider();
  c->control->blockSignals(true);
  c->control->ui().logYCheckBox->setChecked(flag);
  c->control->blockSignals(false);

  create();
}

bool PlotView::isLogY() const {
  return c->yAxisStyle==LOG;
}

bool PlotView::isLogX() const {
  return c->xAxisStyle==LOG;
}

void PlotView::setBox(bool flag){ 
  
  if (flag) {
    c->statBox->show();
    c->statTextItem->show();
  }
  else {
    c->statBox->hide();
    c->statTextItem->hide();
  }
  {
    if (QObject::sender()==c->control->ui().labelBoxBox) c->statBox->setSelected(true);
  }
  c->control->blockSignals(true);
  c->control->ui().labelBoxBox->setChecked(flag);
  c->control->blockSignals(false);
}


QTextEdit *PlotView::titleTextEdit() {return c->titleEditorDialog->textEdit();}
QTextEdit *PlotView::xLabelTextEdit(){return c->xLabelEditorDialog->textEdit();}
QTextEdit *PlotView::yLabelTextEdit(){return c->yLabelEditorDialog->textEdit();}
QTextEdit *PlotView::vLabelTextEdit(){return c->vLabelEditorDialog->textEdit();}
QTextEdit *PlotView::statTextEdit() {return c->statEditorDialog->textEdit();}

void PlotView::updateTitle() {
  QTextEdit *textEdit=c->titleEditorDialog->textEdit();
  QTextDocument *document=textEdit->document();
  c->titleTextItem->setDocument(document);
  QPointF q=c->titleBox->sceneBoundingRect().topLeft();
  c->titleTextItem->setPos(q);
  c->titleTextItem->setTextWidth(c->titleBox->rect().width()); ///!!! HERE
  c->titleTextItem->update();
}

void PlotView::updateStat() {
  QTextEdit *textEdit=c->statEditorDialog->textEdit();
  QTextDocument *document=textEdit->document();
  c->statTextItem->setDocument(document);
  QPointF q=c->statBox->sceneBoundingRect().topLeft()-c->statBox->pos();
  c->statTextItem->setPos(q);
  c->statTextItem->update();
  c->statTextItem->setTextWidth(c->statBox->rect().width());
}


void PlotView::updateXLabel() {
  QTextEdit *textEdit=c->xLabelEditorDialog->textEdit();
  QTextDocument *document=textEdit->document();
  c->xLabelTextItem->setDocument(document);
  QPointF q=c->xLabelBox->sceneBoundingRect().topLeft();
  c->xLabelTextItem->setPos(q);
  c->xLabelTextItem->setTextWidth(c->xLabelBox->rect().width());
  c->xLabelTextItem->update();
}

void PlotView::updateYLabel() {
  QTextEdit *textEdit=c->yLabelEditorDialog->textEdit();
  QTextDocument *document=textEdit->document();
  c->yLabelTextItem->setDocument(document);

  QPointF q=c->yLabelBox->sceneBoundingRect().bottomLeft();
  QMatrix M;
  M.rotate(-90);
  c->yLabelTextItem->setMatrix(M);
  c->yLabelTextItem->setPos(q);
  c->yLabelTextItem->update();

}

void PlotView::updateVLabel() {
  QTextEdit *textEdit=c->vLabelEditorDialog->textEdit();
  QTextDocument *document=textEdit->document();
  c->vLabelTextItem->setDocument(document);
  QPointF q=c->vLabelBox->sceneBoundingRect().bottomLeft();
  QMatrix M;
  M.rotate(-90);
  c->vLabelTextItem->setMatrix(M);
  c->vLabelTextItem->setPos(q);
  c->vLabelTextItem->update();

}


void PlotView::copy() {

  QPen xp=c->xLabelBox->pen();
  QPen yp=c->xLabelBox->pen();
  QPen vp=c->xLabelBox->pen();
  QPen tp =c->titleBox->pen();
  //SM QPen gp =c->graphBox->pen();
  
  xp.setStyle(Qt::NoPen);
  yp.setStyle(Qt::NoPen);
  vp.setStyle(Qt::NoPen);
  tp.setStyle(Qt::NoPen);
  //SM gp.setStyle(Qt::NoPen);

  c->xLabelBox->setPen(xp);
  c->yLabelBox->setPen(yp);
  c->vLabelBox->setPen(vp);
  c->titleBox->setPen(tp);
  //SM c->graphBox->setPen(gp);
 

  QPixmap px = QWidget::grab();
  QClipboard *cb = QApplication::clipboard();
  cb->setPixmap(px);

  if (c->xLabelBox->isVisible()) xp.setStyle(Qt::SolidLine);
  if (c->yLabelBox->isVisible()) yp.setStyle(Qt::SolidLine);
  if (c->vLabelBox->isVisible()) vp.setStyle(Qt::SolidLine);
  if (c->titleBox->isVisible()) tp.setStyle(Qt::SolidLine);


  c->xLabelBox->setPen(xp);
  c->yLabelBox->setPen(yp);
  c->vLabelBox->setPen(vp);
  c->titleBox->setPen(tp);

}

void PlotView::print() {
  QPrinter printer;
  printer.setColorMode(QPrinter::Color);
  printer.setResolution(600);
  printer.setFullPage(true);
  QPrintDialog pDialog(&printer, this);
  if (pDialog.exec()) {

    QPainter paint(&printer);
    QRect viewport=paint.viewport();
    viewport.setTopLeft(QPoint(1000,1500));
    paint.setViewport(viewport);

    copy();
    QPixmap px = QApplication::clipboard()->pixmap();
    QMatrix m;
    m.scale(5,5);
    QPixmap ps = px.transformed(m);

    paint.drawPixmap(0,0,ps);

  }



}


void PlotView::save() {
  QString filename=QFileDialog::getSaveFileName(this, tr("Save File"));
  if (filename!="") {
    if (filename.toStdString().find(".pic")!=std::string::npos) {
      
      QPicture picture;
      picture.setBoundingRect(QWidget::rect());
      QPainter painter;
      painter.begin(&picture);           // paint in picture
      painter.setPen(Qt::NoPen);
      recreate();
      scene()->render (&painter);
      
      painter.end();                     // painting done
      picture.save(filename);            // save picture
      
      
    }
    else if (filename.toStdString().find(".svg")!=std::string::npos) {
      
      QSvgGenerator generator;
      generator.setFileName(filename);
      generator.setSize(size());
      generator.setViewBox(QWidget::rect());
      generator.setResolution(108);
      //generator.setTitle(tr("SVG Generator Example Drawing"));
      //generator.setDescription(tr("An SVG drawing created by the SVG Generator "
      //				  "Example provided with Qt."));
      QPainter painter;
      painter.begin(&generator);
      recreate();
      scene()->render (&painter);
      painter.end();                     
    }
    else {
      copy();
      QPixmap px = QApplication::clipboard()->pixmap();
      px.save(filename);
    }
  }
}

void PlotView::save (const std::string & filename) {
  copy();
  QPixmap px = QApplication::clipboard()->pixmap();
  px.save(QString(filename.c_str()));
  
}



QGraphicsItemGroup *PlotView::group() {
  return c->contentsGroup;
}

QGraphicsScene *PlotView::scene() {
  return &c->scene;
}


QMatrix PlotView::matrix() {
  // Master transformer:
  QMatrix m;
  m.scale(c->graphBoxSize.width()/c->qrect.width(),-c->graphBoxSize.height()/c->qrect.height());
  m.translate(-c->qrect.bottomLeft().x(),  -c->qrect.bottomLeft().y());
  // End master transformer..
  return m;
}
 

void PlotView::add (Plotable *p) {
  c->plotableList.push_back(p);
  create();
}

// Set the Range Divider
void PlotView::setXRangeDivider(AbsRangeDivider * xDivider) {
  c->xRangeDivider=xDivider;
}

// Create or re-create a Range Divider (according to current type)
AbsRangeDivider *PlotView::createXRangeDivider(bool set) {
  if (c->xAllocDivider) delete c->xAllocDivider;
  c->xAllocDivider = c->xAxisStyle==LOG ? (AbsRangeDivider *) new LogRangeDivider : (AbsRangeDivider *) new LinearRangeDivider;
  if (set) c->xRangeDivider=c->xAllocDivider;
  return c->xAllocDivider;
}

// Set the Range Divider
void PlotView::setYRangeDivider(AbsRangeDivider * yDivider) {
  c->yRangeDivider=yDivider;
}

// Create or re-create Range Divider (according to current type).
AbsRangeDivider *PlotView::createYRangeDivider (bool set) {
  if (c->yAllocDivider) delete c->yAllocDivider;
  c->yAllocDivider = c->yAxisStyle==LOG ? (AbsRangeDivider *)  new LogRangeDivider : (AbsRangeDivider *) new LinearRangeDivider;
  if (set) c->yRangeDivider=c->yAllocDivider;
  return c->yAllocDivider;
}

void PlotView::labelHome() {
  c->statBox ->setPos(0,0);
  c->statTextItem->setPos(c->statBox->sceneBoundingRect().topLeft()-c->statBox->pos());
}

void PlotView::editXAxisFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, c->xAxisFont, this);
  if (ok) {
    c->xAxisFont=font;
    create();
    // the user clicked OK and font is set to the font the user selected
  } else {
    // the user canceled the dialog; font is set to the initial
    // value, in this case Helvetica [Cronyx], 10
  }

}

void PlotView::editYAxisFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, c->yAxisFont, this);
  if (ok) {
    c->yAxisFont=font;
    create();
    // the user clicked OK and font is set to the font the user selected
  } else {
    // the user canceled the dialog; font is set to the initial
    // value, in this case Helvetica [Cronyx], 10
  }

}
QFont & PlotView::xAxisFont() const {
  return c->xAxisFont;
}

QFont & PlotView::yAxisFont() const {
  return c->yAxisFont;
}

void PlotView::recreate() {
  create();
}

void PlotView::animate() {
  if (isVisible()) create();
}

bool PlotView::isBox()  const {
  return c->control->ui().labelBoxBox->isChecked();
}
bool PlotView::isGrid() const {
  return c->control->ui().gridCheckBox->isChecked();
}
bool PlotView::isXZero() const {
  return c->control->ui().xZeroCheckBox->isChecked();
}
bool PlotView::isYZero() const {
  return c->control->ui().yZeroCheckBox->isChecked();
}
qreal PlotView::statBoxX() const {
  return c->statBox->x();
}
qreal PlotView::statBoxY() const {
  return c->statBox->y();
}

void PlotView::setLabelPos(qreal x, qreal y) {
  c->statBox ->setPos(x,y);
  c->statTextItem->setPos(c->statBox->sceneBoundingRect().topLeft()-c->statBox->pos());
}

void PlotView::startAnimating(int frameRateMillisec) {
  c->timer->start(frameRateMillisec);
}

void PlotView::stopAnimating() {
  c->timer->stop();
}

void PlotView::setLabelXSizePercentage (qreal percentage) {
  c->xPerCento=percentage;
  c->control->blockSignals(true);
  c->control->setLabelXSizePercentage (int (percentage+0.5));
  c->control->blockSignals(false);
  int w=width(),h=height();
  c->resizeBoxes(w,h);
  c->statBox ->setRect(QRect(c->statBoxTopLeft,c->statBoxSize)); 
  c->statEditorDialog->textEdit()->document()->setTextWidth(c->statBoxSize.width());
  c->statEditorDialog->textEdit()->setFixedWidth((int) c->statBox->rect().width());
}
void PlotView::setLabelYSizePercentage (qreal percentage) {
  c->yPerCento=percentage;
  c->control->blockSignals(true);
  c->control->setLabelYSizePercentage (int (percentage+0.5));
  c->control->blockSignals(false);
  create();
  int w=width(),h=height();
  c->resizeBoxes(w,h);
  c->statBox ->setRect(QRect(c->statBoxTopLeft,c->statBoxSize)); 
  c->statEditorDialog->textEdit()->setFixedHeight((int) c->statBox->rect().height());

}

qreal PlotView::labelXSizePercentage() const {
  return c->xPerCento;
}

qreal PlotView::labelYSizePercentage() const {
  return c->yPerCento;
}
