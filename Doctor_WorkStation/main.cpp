#include "mainwindow.h"
#include <QApplication>
#include <patientmodel.h>
#include <linmainwindow.h>



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    LinMainWindow *win = new LinMainWindow;
    win->show();

    return a.exec();
}
