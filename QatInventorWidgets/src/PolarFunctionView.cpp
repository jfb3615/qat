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

#include "QatInventorWidgets/PolarFunctionView.h"
#include "QatPlotWidgets/ParameterDialog.h"
#include "Inventor/Qt/viewers/SoQtExaminerViewer.h"
#include "QatGenericFunctions/Parameter.h"
#include "QatGenericFunctions/Argument.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoIndexedPointSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/SoOffscreenRenderer.h>
#include "ui_PolarFunctionView.h"
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QAction>
#include <QPixmap>
#include <QPainter>
#include <QScreen>
#include <QFileDialog>
#include <QGuiApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <iostream>
using namespace Genfun;
class PolarFunctionView::Clockwork {
public:

  // User Interface stuff:
  Ui::PolarFunctionView        ui;
  SoQtExaminerViewer          *eviewer          =NULL;
  const AbsFunction           *F                =NULL;

  // Graphics stuff:
  SoSeparator                 *root             =NULL;
  SoSwitch                    *adornSwitch      =NULL;
  SoSwitch                    *antiAdornSwitch  =NULL;
  SoSwitch                    *rulingSwitch     =NULL;
  SoSwitch                    *axesSwitch       =NULL;
  SoSwitch                    *flatAxesSwitch   =NULL;
  SoCoordinate3               *mainCoordinates  =NULL;
  SoNormal                    *mainNormals      =NULL;
  SoMaterial                  *faceMaterial     =NULL;
  SoIndexedFaceSet            *faceSet          =NULL;

  // More graphics stuff for the legend:
  SoSwitch                    *legendSwitch     =NULL;
  SoCoordinate3               *legendCoordinates=NULL;
  SoIndexedFaceSet            *legendFaceSet    =NULL;
  SoNormal                    *legendNormals    =NULL;
  SoMaterial                  *legendMaterial   =NULL;

  // The number of grid points.  
  unsigned int                 N                =64;

  // The squish parameters:
  Parameter                    squishParameter  =Parameter("Squish", 1.0, 0.1, 10.0);
  ParameterDialog             *squishDialog     =NULL;  

  // The following is for printing and saving files:
  QPixmap                      px;
  QFileDialog                  *fDialog         =NULL;
  QPrintDialog                 *pDialog         =NULL;
  QPrinter                      printer;
};


PolarFunctionView::PolarFunctionView(QWidget *parent):QWidget(parent),c(new Clockwork()){
  // Layout this widget:
  c->ui.setupUi(this);

  // Make the viewer
  c->eviewer= new SoQtExaminerViewer(this);
 
  // The basic graphics setup.  
  setupGraphics();

  // Add actions to unroll, toggle axes, toggle longitude lattitude, squish, print, save:
  QAction *actionUnroll = new QAction(this); actionUnroll->setShortcut(QKeySequence("u"));
  actionUnroll->setCheckable(true);
  actionUnroll->setChecked(true);
  this->addAction(actionUnroll);
  connect(actionUnroll, SIGNAL(toggled(bool)), this, SLOT(toggleUnroll(bool)));

  QAction *actionAxes = new QAction(this); actionAxes->setShortcut(QKeySequence("a"));
  actionAxes->setCheckable(true);
  actionAxes->setChecked(false);
  this->addAction(actionAxes);
  connect(actionAxes, SIGNAL(toggled(bool)), this, SLOT(toggleAxes(bool)));

  QAction *actionLatitudeLongitude = new QAction(this); actionLatitudeLongitude->setShortcut(QKeySequence("l"));
  actionLatitudeLongitude->setCheckable(true);
  actionLatitudeLongitude->setChecked(false);
  this->addAction(actionLatitudeLongitude);
  connect(actionLatitudeLongitude, SIGNAL(toggled(bool)), this, SLOT(toggleLatitudeLongitude(bool)));

  c->squishDialog = new ParameterDialog( this, &c->squishParameter);
  connect(c->squishDialog, SIGNAL(updateFunction()), this, SLOT(refreshSphere()));

  QAction *actionSquish = new QAction(this); actionSquish->setShortcut(QKeySequence("h"));
  this->addAction(actionSquish);
  connect(actionSquish, SIGNAL(triggered()), this, SLOT(setSquish()));

  QAction *actionSave = new QAction(this); actionSave->setShortcut(QKeySequence("s"));
  this->addAction(actionSave);
  connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

  QAction *actionPrint = new QAction(this); actionPrint->setShortcut(QKeySequence("p"));
  this->addAction(actionPrint);
  connect(actionPrint, SIGNAL(triggered()), this, SLOT(print()));

  // Set the roll/unroll to desired initial postions:
  toggleUnroll(actionUnroll->isChecked());


  //
  //These are for printing and saving.
  //
  c->fDialog=new QFileDialog(this);
  c->fDialog->setOption(QFileDialog::ShowDirsOnly , true);
  c->fDialog->setOption(QFileDialog::HideNameFilterDetails,true );
  c->fDialog->setViewMode(QFileDialog::List);
  connect(c->fDialog,SIGNAL(fileSelected(const QString &)), this, SLOT(save(const QString &)));

  c->printer.setColorMode(QPrinter::Color);
  c->printer.setResolution(600);
  c->printer.setFullPage(true);
  
  c->pDialog=new QPrintDialog(&c->printer,this);
}

// Destructor:
PolarFunctionView::~PolarFunctionView(){
 
  c->root->unref();
  delete c->eviewer;
  delete c;
}

// Probably not needed. 
void PolarFunctionView::resizeEvent (QResizeEvent */*event*/){
}

// Actual save:
void PolarFunctionView::save (const std::string & filename){
  c->px.save(filename.c_str());
}

// Callback:
void PolarFunctionView::save (const QString & path){
  save(path.toStdString());
}

// Open a dialog and choose the save file name:
void PolarFunctionView::save(){
  raise();
  c->px=QGuiApplication::primaryScreen()->grabWindow(c->eviewer->getWidget()->winId());
  c->fDialog->move(width()+100,0);
  c->fDialog->open();
}

// Open a dialog and print:
void PolarFunctionView::print(){
  raise();
  c->px=QGuiApplication::primaryScreen()->grabWindow(c->eviewer->getWidget()->winId());
  c->pDialog->move(width()+100,0);
  if (c->pDialog->exec()) {
    QPainter paint(&c->printer);
    QRect viewport=paint.viewport();
    viewport.setTopLeft(QPoint(200,500));
    paint.setViewport(viewport);
    
    QMatrix m;
    m.scale(6,6);
    QPixmap ps = c->px.transformed(m);

    paint.drawPixmap(0,0,ps);
  }
}

// Set the function:
void PolarFunctionView::setFunction (const AbsFunction * F){
  c->F=F;
  if (F) refreshSphere();
}


// This initialized the graphis:
void PolarFunctionView::setupGraphics() {
  c->root = new SoSeparator;
  c->root->ref();

  c->adornSwitch = new SoSwitch();
  c->adornSwitch->whichChild=SO_SWITCH_ALL;

  c->antiAdornSwitch = new SoSwitch();
  c->antiAdornSwitch->whichChild=SO_SWITCH_NONE;

  c->rulingSwitch = new SoSwitch();
  SoSeparator *rulingSeparator = new SoSeparator;
  c->rulingSwitch->addChild(rulingSeparator);
  c->adornSwitch->addChild(c->rulingSwitch);
  c->rulingSwitch->whichChild=SO_SWITCH_NONE;


  SoSphere *sphere = new SoSphere();
  sphere->radius=1.075;
  SoMaterial *blue = new SoMaterial();   
  blue->diffuseColor.setValue(0,0,1);      
  SoLightModel *rulingModel=new SoLightModel;
  rulingModel->model=SoLightModel::BASE_COLOR;
  SoDrawStyle *rulingDrawStyle=new SoDrawStyle;
  SoRotationXYZ *rulingRot=new SoRotationXYZ();
  rulingRot->axis=SoRotationXYZ::X;
  rulingRot->angle=M_PI/2;
  rulingDrawStyle->style=SoDrawStyle::LINES;
  rulingDrawStyle->lineWidth=1;
  rulingSeparator->addChild(rulingRot);
  rulingSeparator->addChild(rulingModel);
  rulingSeparator->addChild(rulingDrawStyle);
  rulingSeparator->addChild(blue);
  rulingSeparator->addChild(sphere);


  

  c->mainCoordinates = new SoCoordinate3;
  c->faceSet=new SoIndexedFaceSet;
  SoMaterialBinding *binding = new SoMaterialBinding;
  c->mainNormals = new SoNormal;
  c->faceMaterial=new SoMaterial;

  
  unsigned int N=c->N;

  binding->value=SoMaterialBinding::PER_VERTEX_INDEXED;
  double x0= 0, x1=M_PI,     dx=(x1-x0)/double(N);;
  double p0= 0, p1=2.0*M_PI, dp=(p1-p0)/double(N);;
  SoLightModel *model=new SoLightModel;
  model->model=SoLightModel::PHONG;
  unsigned int index=0;

  
  c->legendSwitch=new SoSwitch();
  c->legendSwitch->whichChild=SO_SWITCH_ALL;
  c->root->addChild(c->legendSwitch);
  for (unsigned int i=0;i<N+1;i++) {
    double x = x0+i*dx;
    for (unsigned int j=0;j<N;j++) {
      double p = p0+j*dp;

      double COSTHETA=cos(x);
      double SINTHETA=sin(x);
      
      double X=SINTHETA*cos(p), Y=SINTHETA*sin(p), Z=COSTHETA;
      X*=1.05; Y*=1.05; Z*=1.05;

      c->mainCoordinates->point.set1Value(index, X,Y,Z);
      c->mainNormals->vector.set1Value(index,X,Y,Z);

      // Blank sphere. Coloured in "refresh"
      c->faceMaterial->diffuseColor.set1Value(index,1,1,1);

      index++;
    }
  }
  //
  // Now index
  //
  unsigned int counter=0;
  for (unsigned int i=0;i<N;i++) {
    for (unsigned int j=0;j<N;j++) {
      
      unsigned int base0 = N*i+j;
      unsigned int base1 = N*(i+1)+j;
      unsigned int base2 = j+1==N ? N*i     : base0+1;
      unsigned int base3 = j+1==N ? N*(i+1) : base1+1;
      
      c->faceSet->materialIndex.set1Value(counter,base0);
      c->faceSet->normalIndex.set1Value(counter,base0);
      c->faceSet->coordIndex.set1Value(counter ,base0);
      counter++;


      c->faceSet->materialIndex.set1Value(counter,base1);
      c->faceSet->normalIndex.set1Value(counter,base1);
      c->faceSet->coordIndex.set1Value(counter ,base1);
      counter++;


      c->faceSet->materialIndex.set1Value(counter,base2); 
      c->faceSet->normalIndex.set1Value(counter,base2); 
      c->faceSet->coordIndex.set1Value(counter ,base2); 
      counter++;


      c->faceSet->materialIndex.set1Value(counter,-1);
      c->faceSet->normalIndex.set1Value(counter,-1);
      c->faceSet->coordIndex.set1Value(counter ,-1);
      counter++;


      c->faceSet->materialIndex.set1Value(counter,base1);
      c->faceSet->normalIndex.set1Value(counter,base1);
      c->faceSet->coordIndex.set1Value(counter ,base1);
      counter++;


      c->faceSet->materialIndex.set1Value(counter,base3);
      c->faceSet->normalIndex.set1Value(counter,base3);
      c->faceSet->coordIndex.set1Value(counter ,base3);
      counter++;


      c->faceSet->materialIndex.set1Value(counter,base2); 
      c->faceSet->normalIndex.set1Value(counter,base2); 
      c->faceSet->coordIndex.set1Value(counter ,base2); 
      counter++;


      c->faceSet->materialIndex.set1Value(counter,-1);
      c->faceSet->normalIndex.set1Value(counter,-1);
      c->faceSet->coordIndex.set1Value(counter ,-1);
      counter++;


    }
  }

  //c->faceSet->materialIndex.set1Value(counter,-2);
  //c->faceSet->normalIndex.set1Value(counter,-2);
  //c->faceSet->coordIndex.set1Value(counter ,-2);
  counter++;

  {
    SoRotationXYZ *rot=new SoRotationXYZ;
    rot->axis=SoRotationXYZ::X;
    rot->angle=-M_PI/2.0;
    c->root->addChild(rot);
  }
  {
    SoRotationXYZ *rot=new SoRotationXYZ;
    rot->axis=SoRotationXYZ::Z;
    rot->angle=-M_PI/2.0;
    c->root->addChild(rot);
  }

  c->root->addChild(c->adornSwitch);
  c->root->addChild(c->antiAdornSwitch);
  c->root->addChild(model);
  c->root->addChild(c->mainCoordinates);
  c->root->addChild(c->mainNormals);
  c->root->addChild(c->faceMaterial);
  c->root->addChild(binding);
  c->root->addChild(c->faceSet);

  c->axesSwitch = new SoSwitch();
  SoSeparator *axesSeparator = new SoSeparator;
  c->axesSwitch->addChild(axesSeparator);
  c->adornSwitch->addChild(c->axesSwitch);
  c->axesSwitch->whichChild=SO_SWITCH_NONE;
  
  SoLightModel *lModel=new SoLightModel;
  lModel->model=SoLightModel::BASE_COLOR;
  SoDrawStyle *drawStyle=new SoDrawStyle;
  drawStyle->style=SoDrawStyle::LINES;
  drawStyle->lineWidth=3;
  axesSeparator->addChild(lModel);
  axesSeparator->addChild(drawStyle);
  std::string label[]={"X", "Y", "Z"};

  for (int i=0;i<3;i++)
  {
    SoSeparator *sep = new SoSeparator;
    SoCoordinate3 *coord = new SoCoordinate3;
    SoLineSet *lineSet=new SoLineSet;
    SoMaterial *material=new SoMaterial;
    coord->point.set1Value(0,SbVec3f(0,0,0));
    coord->point.set1Value(1,SbVec3f((i==0 ? 1.5:0), (i==1? 1.5: 0), (i==2 ? 1.5: 0)));
    material->diffuseColor.setValue((i==0 ? 1.5:0), (i==1? 1.5: 0), (i==2 ? 1.5: 0));
    axesSeparator->addChild(sep);
    sep->addChild(material);
    sep->addChild(coord);
    sep->addChild(lineSet);
    SoTranslation *translation = new SoTranslation;
    translation->translation.setValue((i==0 ? 1.52:0), (i==1? 1.52: 0), (i==2 ? 1.52: 0));
    SoFont  *font= new SoFont;
    font->size=32;
    
    SoText2 *text = new SoText2;
    text->string=label[i].c_str();
    sep->addChild(translation);
    sep->addChild(font);
    sep->addChild(text);
  
  }


  c->flatAxesSwitch = new SoSwitch();
  SoSeparator *flatAxesSeparator = new SoSeparator;
  c->flatAxesSwitch->addChild(flatAxesSeparator);
  c->flatAxesSwitch->whichChild=SO_SWITCH_NONE;
  c->antiAdornSwitch->addChild(c->flatAxesSwitch);
  flatAxesSeparator->addChild(lModel);
  flatAxesSeparator->addChild(drawStyle);
  SoCoordinate3 *coord = new SoCoordinate3;
  SoLineSet *lineSet=new SoLineSet;
  SoMaterial *material=new SoMaterial;
  coord->point.set1Value(0,SbVec3f(0.05,0,0));
  coord->point.set1Value(1,SbVec3f(0.05,-2*sqrt(2.0),0));
  material->diffuseColor.setValue(1,1,1);
  flatAxesSeparator->addChild(material);
  flatAxesSeparator->addChild(coord);
  flatAxesSeparator->addChild(lineSet);
  SoFont  *font= new SoFont;
  font->size=32;
  flatAxesSeparator->addChild(font);
  SbVec3f tr[]={SbVec3f(0.05,-2*sqrt(2.0),0), SbVec3f(0.05,-sqrt(2.0),0), SbVec3f(0.05,0, sqrt(2.0))};
  for (int i=0;i<3;i++) {
    SoSeparator *sep=new SoSeparator;
    SoText2 *text = new SoText2;
    text->string=label[i].c_str();
    SoTranslation *translation = new SoTranslation;
    translation->translation.setValue(tr[i]);
    sep->addChild(translation);
    sep->addChild(text);
    flatAxesSeparator->addChild(sep);
  }

  {
    SoSeparator *legendSeparator = new SoSeparator;
    c->legendSwitch->addChild(legendSeparator);
     
    c->legendCoordinates = new SoCoordinate3;
    c->legendFaceSet=new SoIndexedFaceSet;
    c->legendNormals = new SoNormal;
    c->legendMaterial=new SoMaterial;

    SoMaterialBinding *binding = new SoMaterialBinding;
    binding->value=SoMaterialBinding::PER_VERTEX_INDEXED;

     
    unsigned int counter=0;
    double squish=c->squishParameter.getValue();
    for (int i=0;i<10;i++) {
      for (int l=0;l<2;l++) {
	for (int j=0;j<2;j++) {
	  c->legendFaceSet->materialIndex.set1Value(counter,4*i+2*l+j);
	  c->legendFaceSet->normalIndex.set1Value(counter,4*i+2*l+j);
	  c->legendFaceSet->coordIndex.set1Value(counter,4*i+2*l+j);
	  c->legendCoordinates->point.set1Value(4*i+2*l+j, 2.85+ (j+l)%2/4.0, i/4.0+l/4.0-1.25,0);
	  c->legendNormals->vector.set1Value(4*i+2*l+j, 0, 0, 1);
	  
	  
	  double y = (i+l/2.0)/16;
	  
	  double R=tanh(squish*(y));
	  double G=tanh(squish*(1-y));
	  double B=tanh(squish*(1-y));
 
	  c->legendMaterial->diffuseColor.set1Value(4*i+2*l+j, R,G,B);
	  counter++;
	}
      }
      c->legendFaceSet->materialIndex.set1Value(counter,-1);
      c->legendFaceSet->normalIndex.set1Value(counter,-1);
      c->legendFaceSet->coordIndex.set1Value(counter ,-1);
      counter++;
      
    }
    legendSeparator->addChild(c->legendCoordinates);
    legendSeparator->addChild(c->legendNormals);
    legendSeparator->addChild(c->legendMaterial);
    legendSeparator->addChild(binding);
    legendSeparator->addChild(c->legendFaceSet);
  }
  
  //
  //-------------------------------------------------------------
  // Use one of the convenient SoQt viewer classes.
  c->eviewer->setSceneGraph(c->root);
  c->eviewer->setDecoration(false);
  c->eviewer->viewAll();
}


void PolarFunctionView::setSquish(){
  c->squishDialog->move(0,0);
  c->squishDialog->setWindowTitle(QApplication::translate("Squish", "Squish Factor"));
  c->squishDialog->show();

}
void PolarFunctionView::refreshSphere(){
  //
  // Refresh the Crystal Ball:
  //
  double squish=c->squishParameter.getValue();
  double x0= 0, x1=M_PI,     dx=(x1-x0)/double(c->N);;
  double p0= 0, p1=2.0*M_PI, dp=(p1-p0)/double(c->N);;
  bool entered=false;
  while (!entered) {
    unsigned int index=0;
    entered=true;
    double mx=-100.0, mn=100.0;
    for (unsigned int i=0;i<c->N+1;i++) {
      double x = x0+i*dx;
      for (unsigned int j=0;j<c->N;j++) {
        double p = p0+j*dp;
        double COSTHETA=cos(x);
        Argument arg(2); arg[0]=COSTHETA; arg[1]=p;
	double y=(*c->F)(arg);
	mn=std::min(mn,y);
	mx=std::max(mx,y);
      }
    }
    double span=(mx-mn);
    
    for (unsigned int i=0;i<c->N+1;i++) {
      double x = x0+i*dx;
      for (unsigned int j=0;j<c->N;j++) {
        double p = p0+j*dp;
        double COSTHETA=cos(x);
        Argument arg(2); arg[0]=COSTHETA; arg[1]=p;
	double y=span==0 ? 0.5: ((*c->F)(arg)-mn)/span;
        double R=tanh(squish*(y));
        double G=tanh(squish*(1-y));
        double B=tanh(squish*(1-y));
        c->faceMaterial->diffuseColor.set1Value(index,R,G,B);
        index++;
      }
    }
    // Remake the legend, too:
    for (int i=0;i<10;i++) {
      for (int l=0;l<2;l++) {
	for (int j=0;j<2;j++) {
	  double y = (i+l/2.0)/10;
	  double R=tanh(squish*(y));
	  double G=tanh(squish*(1-y));
	  double B=tanh(squish*(1-y));
	  c->legendMaterial->diffuseColor.set1Value(4*i+2*l+j, R,G,B);
	}
      }
    }
  }
}
void PolarFunctionView::toggleLatitudeLongitude(bool flag){
  c->rulingSwitch->whichChild=flag? SO_SWITCH_ALL:SO_SWITCH_NONE;
}
void PolarFunctionView::toggleAxes(bool flag){
  c->axesSwitch->whichChild=flag? SO_SWITCH_ALL:SO_SWITCH_NONE;
  c->flatAxesSwitch->whichChild=flag? SO_SWITCH_ALL:SO_SWITCH_NONE;
}
void PolarFunctionView::toggleUnroll(bool flag){
  //    flag = !flag;
  
  c->legendSwitch->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE; 
  c->adornSwitch->whichChild=flag ? SO_SWITCH_NONE:SO_SWITCH_ALL;
  c->antiAdornSwitch->whichChild=flag ? SO_SWITCH_ALL:SO_SWITCH_NONE;

  unsigned int N=c->N;
  double dd=0.001;
  double x0= 0+dd, x1=M_PI-dd,     dx=(x1-x0)/double(N);;
  double p0= 0, p1=2.0*M_PI,       dp=(p1-p0)/double(N);;
  unsigned int index=0;
  for (unsigned int i=0;i<N+1;i++) {
    double x = x0+i*dx;
    for (unsigned int j=0;j<N;j++) {
      double p = p0+j*dp;
      double latitude=M_PI/2-x;
      double alpha = latitude;
      double alphaPrime = 2*alpha;
      while (1) {
        double delta = -(alphaPrime + sin(alphaPrime) - M_PI*sin(latitude))/(1+cos(alphaPrime));
        if (fabs(delta)<1E-6) break; 
        alphaPrime += delta;
      }
      alpha=alphaPrime/2;
      double COSTHETA=cos(x);
      double SINTHETA=sin(x);
      double X  = flag ? 0                                   : 1.05*SINTHETA*cos(p);
      double Y  = flag ?   2*sqrt(2)*(p-M_PI)*cos(alpha)/M_PI       : 1.05*SINTHETA*sin(p); 
      double Z  = flag ?   sqrt(2)*sin(alpha)                : 1.05*COSTHETA;
      double nX = flag ? 1.0                                 : X;
      double nY = flag ? 0                                   : Y;
      double nZ = flag ? 0                                   : Z;
  
      c->mainCoordinates->point.set1Value(index, X,Y,Z);
      c->mainNormals->vector.set1Value(index,nX,nY,nZ);
      
      
      index++;
    }
  }
  if (flag)  {
    unsigned int counter=0;
    for (unsigned int i=0;i<N;i++) {
      for (unsigned int j=0;j<N;j++) {
        
        unsigned int base0 = N*i+j;
        unsigned int base1 = N*(i+1)+j;
        unsigned int base2 = j+1==N ? 0 : base0+1;
        unsigned int base3 = j+1==N ? 0 : base1+1;

        if (base2!=0) {
          c->faceSet->materialIndex.set1Value(counter,base0);
          c->faceSet->normalIndex.set1Value(counter,base0);
          c->faceSet->coordIndex.set1Value(counter ,base0);
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,base1);
          c->faceSet->normalIndex.set1Value(counter,base1);
          c->faceSet->coordIndex.set1Value(counter ,base1);
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,base2); 
          c->faceSet->normalIndex.set1Value(counter,base2); 
          c->faceSet->coordIndex.set1Value(counter ,base2); 
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,-1);
          c->faceSet->normalIndex.set1Value(counter,-1);
          c->faceSet->coordIndex.set1Value(counter ,-1);
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,base1);
          c->faceSet->normalIndex.set1Value(counter,base1);
          c->faceSet->coordIndex.set1Value(counter ,base1);
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,base3);
          c->faceSet->normalIndex.set1Value(counter,base3);
          c->faceSet->coordIndex.set1Value(counter ,base3);
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,base2); 
          c->faceSet->normalIndex.set1Value(counter,base2); 
          c->faceSet->coordIndex.set1Value(counter ,base2); 
          counter++;
          
          
          c->faceSet->materialIndex.set1Value(counter,-1);
          c->faceSet->normalIndex.set1Value(counter,-1);
          c->faceSet->coordIndex.set1Value(counter ,-1);
          counter++;
        }          
        
      }
    }
    
    //c->faceSet->materialIndex.set1Value(counter,-2);
    //c->faceSet->normalIndex.set1Value(counter,-2);
    //c->faceSet->coordIndex.set1Value(counter ,-2);
    counter++;
  }
  else {
    unsigned int counter=0;
    for (unsigned int i=0;i<N;i++) {
      for (unsigned int j=0;j<N;j++) {
        
        unsigned int base0 = N*i+j;
        unsigned int base1 = N*(i+1)+j;
        unsigned int base2 = j+1==N ? N*i     : base0+1;
        unsigned int base3 = j+1==N ? N*(i+1) : base1+1;

        c->faceSet->materialIndex.set1Value(counter,base0);
        c->faceSet->normalIndex.set1Value(counter,base0);
        c->faceSet->coordIndex.set1Value(counter ,base0);
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,base1);
        c->faceSet->normalIndex.set1Value(counter,base1);
        c->faceSet->coordIndex.set1Value(counter ,base1);
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,base2); 
        c->faceSet->normalIndex.set1Value(counter,base2); 
        c->faceSet->coordIndex.set1Value(counter ,base2); 
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,-1);
        c->faceSet->normalIndex.set1Value(counter,-1);
        c->faceSet->coordIndex.set1Value(counter ,-1);
        counter++;

        
        c->faceSet->materialIndex.set1Value(counter,base1);
        c->faceSet->normalIndex.set1Value(counter,base1);
        c->faceSet->coordIndex.set1Value(counter ,base1);
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,base3);
        c->faceSet->normalIndex.set1Value(counter,base3);
        c->faceSet->coordIndex.set1Value(counter ,base3);
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,base2); 
        c->faceSet->normalIndex.set1Value(counter,base2); 
        c->faceSet->coordIndex.set1Value(counter ,base2); 
        counter++;
        
        
        c->faceSet->materialIndex.set1Value(counter,-1);
        c->faceSet->normalIndex.set1Value(counter,-1);
        c->faceSet->coordIndex.set1Value(counter ,-1);
        counter++;

        
      }
    }
    
    //c->faceSet->materialIndex.set1Value(counter,-2);
    //c->faceSet->normalIndex.set1Value(counter,-2);
    //c->faceSet->coordIndex.set1Value(counter ,-2);
    counter++;
  }
  c->eviewer->viewAll();
  if (flag) {
    c->eviewer->getCamera()->position.setValue(0,0,5.5);
  }
}
SoQtExaminerViewer *PolarFunctionView::getViewer() const {
  return c->eviewer;
}
