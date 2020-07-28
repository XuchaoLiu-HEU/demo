#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include "wavewindow.h"
#include "numberwindow.h"
#include "menuwindow.h"
#include "serialrx.h"
#include "mytcpsocket.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void transitData(int ecg, int spo, int ibp);
    void setHeartRate(int hr);
    void tcpNewConnection();
    void handleTcpData(QStringList info);

private:
    Ui::MainWindow *ui;

    MenuWindow *menuWindow;

    WaveWindow *ecgWave;
    WaveWindow *spoWave;
    WaveWindow *ibpWave;

    NumberWindow *hrWindow;
    NumberWindow *stWindow;
    NumberWindow *nibpWindow;
    NumberWindow *spoWindow;
    NumberWindow *tempWindow;
    NumberWindow *respWindow;
    NumberWindow *co2Window;

    SerialRx *rx;
    QTcpServer *mServer;
    QTcpSocket *mainSocket;
};
#endif // MAINWINDOW_H
