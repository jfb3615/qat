#include "QatDataAnalysis/OptParse.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0] + " [Input1] [Input2] ... -o Output [-v]"; 
  if (argc==1) {
    std::cout << usage << std::endl;
    exit(0);
  }

  HIONToOne hio;
  try {
    hio.optParse(argc,argv);
  }
  catch (std::exception & e) {
    std::cerr << "Error " << e.what()  << std::endl;
    std::cerr << usage    << std::endl << std::endl;
    exit(0);
  }
  if (argc!=1) {      
    std::cerr << "Unrecognized option: " << argv[1] <<std::endl;
    exit(0);
  }

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, SIGNAL(triggered()), &app, SLOT(quit()));
  
  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(1.0);
  rect.setYmin(0.0);
  rect.setYmax(1.0);
  

  PlotView view(rect);
  window.setCentralWidget(&view);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Sans Serif") 
	      << PlotStream::Size(16)
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Sans Serif")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();
  
  
  
  window.show();
  app.exec();
  return 1;
}

