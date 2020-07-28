#include "mainwindow.h"

#include <QApplication>
#include "menuwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    MenuWindow *w = new MenuWindow();
//    w->show();

    return a.exec();
}
