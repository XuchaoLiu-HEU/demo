#ifndef SERIALTX_H
#define SERIALTX_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include "realData.h"

class SerialTx: public QObject
{
    Q_OBJECT
public:
    SerialTx();
    ~SerialTx();
private:
    QSerialPort *com;
    QTimer *timer;
    int index[3];

    char bccCheck(char *buffer, int len);
private slots:
    void sendData();
};

#endif // SERIALTX_H
