TEMPLATE = subdirs
  SUBDIRS  = QatGenericFunctions \
  QatDataAnalysis \
  QatDataModeling \
  QatPlotting \
  QatPlotWidgets \
  QatProject \
  Qhfd

QatGenericFunctions.file=QatGenericFunctions/src/qt.pro
QatDataAnalysis.file=QatDataAnalysis/src/qt.pro
QatDataModeling.file=QatDataModeling/src/qt.pro
QatPlotting.file=QatPlotting/src/qt.pro
QatPlotWidgets.file=QatPlotWidgets/src/qt.pro
QatProject.file=QatProject/qt.pro
Qhfd.file=Qhfd/src/qt.pro

message (Configuring QAT packages)


!isEmpty(QAT3D) {
  message (Configuring Coin-based QAT3D packages)
  SUBDIRS += QatInventorWidgets
  QatInventorWidgets.file = QatInventorWidgets/src/qt.pro
}
