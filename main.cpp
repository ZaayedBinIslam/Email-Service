#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  // qDebug() << "Available styles:" << QStyleFactory::keys();
  // a.setStyle(QStyleFactory::create("Windows"));
  MainWindow w;
  w.show();
  return a.exec();
}
