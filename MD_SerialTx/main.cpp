#include "mainwindow.h"

#include <QApplication>
#include "serialtx.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();

    SerialTx *tx = new SerialTx();
    return a.exec();
}
