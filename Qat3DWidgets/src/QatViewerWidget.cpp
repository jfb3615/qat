#include "Qat3DWidgets/QatViewerWidget.h"
#include <Qt3DWindow>
#include <QHBoxLayout>
#include <QCamera>
#include <QCameraLens>
#include <iostream>
#include <cmath>
#include <iostream>
#include "ui_QatViewerWidget.h"

class QatViewerWidget::Clockwork {

public:
  
  Ui::QatViewerWidget     ui;
  Qt3DExtras::Qt3DWindow *wind;
  
  Qt3DRender::QCamera                *camera;
  Qt3DExtras::QOrbitCameraController *camController; 
  Qt3DRender::QObjectPicker          *objectPicker;
  Qt3DRender::QDirectionalLight      *headlight;
  
  QTimer              *seekTimer;
  QVector3D            vAve;
  QVector3D            vDel;
  double               x;
  
  QVector3D            homeViewCenter;
  QVector3D            homePosition;
};
    

QatViewerWidget::~QatViewerWidget() {
  delete c;
}

QatViewerWidget::QatViewerWidget(QWidget *parent):
  QWidget(parent), c(new Clockwork()) {

  c->ui.setupUi(this);
  c->wind=new Qt3DExtras::Qt3DWindow();
  QHBoxLayout *layout=new QHBoxLayout(c->ui.widget);
  QWidget *w2=QWidget::createWindowContainer(c->wind,c->ui.widget);
  layout->addWidget(w2);

  // Camera
  c->camera = c->wind->camera();
  c->camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
  c->camera->setPosition(QVector3D(0, 0, 40.0f));
  c->camera->setViewCenter(QVector3D(0, 0, 0));


  c->homeViewCenter=c->camera->viewCenter();
  c->homePosition=c->camera->position();
  
  // For object picking
  c->objectPicker=new  Qt3DRender::QObjectPicker();

  c->seekTimer = new QTimer();
  c->seekTimer->setInterval(10);

  c->headlight=new Qt3DRender::QDirectionalLight;
  
  
  QObject::connect(c->seekTimer,SIGNAL(timeout()),this, SLOT(handleSeek()));
  QObject::connect(c->ui.seekButton, SIGNAL(clicked()), this, SLOT(waitForSeek()));
  QObject::connect(c->ui.omniButton, SIGNAL(clicked()), this, SLOT(viewAll()));
  QObject::connect(c->ui.setHomeButton, SIGNAL(clicked()), this, SLOT(setHome()));
  QObject::connect(c->ui.goHomeButton, SIGNAL(clicked()), this, SLOT(goHome()));
  QObject::connect(c->objectPicker,
		   SIGNAL(clicked(Qt3DRender::QPickEvent *)),
		   this,
		   SLOT(handlePick(Qt3DRender::QPickEvent *)));
}

void QatViewerWidget::setRootEntity( Qt3DCore::QEntity *rootEntity) {
  rootEntity->removeComponent(c->headlight);
  rootEntity->addComponent(c->objectPicker);
  c->wind->setRootEntity(rootEntity);

  // For camera controls
  c->camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
  c->camController->setLinearSpeed(200.0f );
  c->camController->setLookSpeed  (200.0f );
  c->camController->setCamera(c->camera);

  // The headlight is not automatically part of the viewer. So we have to add it explicity.
  
  c->headlight->setColor(QColor("white"));
  c->headlight->setIntensity(1);
  c->headlight->setWorldDirection({0,0,-1});
  rootEntity->addComponent(c->headlight);
  
  QObject::connect( c->camera, SIGNAL(positionChanged(const QVector3D &)), this, SLOT(cameraAngleChanged(const QVector3D &)));
  
}

void QatViewerWidget::waitForSeek() {
  c->ui.seekButton->setEnabled(false);
}

void QatViewerWidget::handleSeek() {

  double y=erf(c->x);
  QVector3D v=c->vAve+y*c->vDel;
  c->camera->setViewCenter(v);
  if (c->x>2.0) {
    c->seekTimer->stop();
    c->ui.seekButton->setEnabled(true);
  }
  else {
    c->x+=0.01;
  }
}

void QatViewerWidget::handlePick(Qt3DRender::QPickEvent *pick) {
  
  if (!c->ui.seekButton->isEnabled()) {
    QVector3D vCenter=c->camera->viewCenter();
    QVector3D vPick  = pick->worldIntersection();
    c->vAve   = (vPick+vCenter)/2.0;
    c->vDel   = (vPick-vCenter)/2.0;
    c->x=-2.0;
    c->seekTimer->start();
  }
  else {
  }
}

void QatViewerWidget::viewAll() {
  c->camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
  c->camera->setPosition(QVector3D(0, 0, 40.0f));
  c->camera->setViewCenter(QVector3D(0, 0, 0));
}

void QatViewerWidget::cameraAngleChanged(const QVector3D &v) {
  c->headlight->setWorldDirection(-v);
}

void QatViewerWidget::setHome() {
  c->homeViewCenter=c->camera->viewCenter();
  c->homePosition=c->camera->position();
}

void QatViewerWidget::goHome() {
  c->camera->setViewCenter(c->homeViewCenter);
  c->camera->setPosition(c->homePosition);
}

const Qt3DRender::QCamera *QatViewerWidget::getCamera() const {
  return c->camera;
}

Qt3DRender::QCamera *QatViewerWidget::getCamera() {
  return c->camera;
}
