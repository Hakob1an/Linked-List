// main.cpp
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
    w.show();
    return a.exec();
}
