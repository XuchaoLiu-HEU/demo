#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "realdata.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket(QObject *parent = nullptr);

signals:
    void patientReadFinish(QStringList info);

private slots:
    void myReadyRead();
};

#endif // MYTCPSOCKET_H
