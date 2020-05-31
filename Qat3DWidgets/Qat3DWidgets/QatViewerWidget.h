#ifndef  _QATVIEWERWIDGET_H_
#define  _QATVIEWERWIDGET_H_
#include <Qt3DWindow>
#include <QObjectPicker>
#include <QPickEvent>
#include <QTimer>
#include <Qt3DWindow>
#include <QOrbitCameraController>
#include <QCamera>
#include <QFirstPersonCameraController>
#include <Qt3DRender/QDirectionalLight>
#include <QWidget>
class QatViewerWidget: public QWidget {

  Q_OBJECT
  
 public:
  
  QatViewerWidget(QWidget *parent=NULL);

  virtual ~QatViewerWidget();
  
  // Set root entity (or "scene");
  void setRootEntity( Qt3DCore::QEntity *);

  // Access to the camera:
  Qt3DRender::QCamera *getCamera();
  const Qt3DRender::QCamera *getCamera() const;

  
  public slots:

    void waitForSeek();
    
    void handleSeek(); 
    
    void handlePick(Qt3DRender::QPickEvent *pick); 

    void viewAll();

    void cameraAngleChanged(const QVector3D &);

    void setHome();

    void goHome();
    
 private:
    
    // It is illegal to copy or assign:
    QatViewerWidget(const QatViewerWidget &)              = delete;
    QatViewerWidget & operator= (const QatViewerWidget &) = delete;

    class Clockwork;
    Clockwork *c;

};
#endif

