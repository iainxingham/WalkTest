#define MAIN_CPP

#include "mainwindow.h"
#include "dbobj.h"

#include <QApplication>

DBObj *ClinicalDB = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
