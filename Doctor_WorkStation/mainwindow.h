#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <patientmodel.h>
#include <QTcpSocket>
#include <timewindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool connectDB();
    static int ecgw[500];
    static int spow[500];
    static int ibpw[500];
private slots:
    void tcpReadyRead();    //槽函数，用来绑定socket的readyRead（）信号


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

signals:
    void sendsignal();

private:
    Ui::MainWindow *ui;
    PatientModel patientsDev;
    Timewindow patientstime;
    QTcpSocket *mSocket;
};

#endif // MAINWINDOW_H
