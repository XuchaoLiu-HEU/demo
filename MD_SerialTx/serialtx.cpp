#include "serialtx.h"
#include <QDebug>

SerialTx::SerialTx()
{
    com = new QSerialPort();
    com->setPortName("com1");
    if (!com->open(QIODevice::ReadWrite)){
        qDebug() << "com open error";
        return;
    }

    qDebug() << "com open success";
    com->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
    com->setStopBits(QSerialPort::OneStop);

    timer = new QTimer();
    timer->start(10);            // 每秒1000 / 2 数
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));

    for (int i = 0; i < 3; i++){
        index[i] = 0;
    }
}

SerialTx::~SerialTx()
{
    timer->stop();
    timer->deleteLater();
    com->deleteLater();
}

char SerialTx::bccCheck(char *buffer, int len)
{
    // buffer长为len
    char bcc = 0x00;
    for (int i = 0; i < len - 1; i++){
        bcc ^= buffer[i];
    }
    return bcc;
}

void SerialTx::sendData()
{
    // 包头：1位，数据头：1位，ECG：2位，SPO：1位，IBP：1位，校验：1位
    // 1 + 1 + 2 + 1 + 1 + 1 = 7
    //qDebug("sending...");

    char buffer[7];
    int data[3] = {Ecg_Data[index[0]], Spo_Data[index[1]], Ibp_Data[index[2]]};

    buffer[0] = 0x08;           // 数据类别，最高位一定为0
    buffer[1] = 0x80;           // 数据头，最高位一定为1，其余7为存储数据的最高位，最低为无效

    // ECG数据
    buffer[1] = buffer[1] | ((data[0]&0x8000) >> 9);
    buffer[1] = buffer[1] | ((data[0]&0x80) >> 2);
    buffer[2] = 0x80 | (0x7F & (data[0] >> 8));
    buffer[3] = 0x80 | (0x7F & data[0]);

    // SPO, IBP数据
    for(int i = 1; i < 3; i++){
        buffer[1] = buffer[1] | ((data[i]&0x80) >> (i + 2));
        buffer[i + 3] = 0x80 | (0x7F & data[i]);
    }

    buffer[6] = bccCheck(buffer, 7); //校验位

    // debug, 验证正确
//    for (int i = 0; i < 3; i++) {
//        qDebug("%d", data[i]);
//    }
//    for (int i = 0; i < 7; i++) {
//        qDebug("%x", buffer[i]);
//    }
//    for (int i = 0; i < 3; i++){
//        qDebug("index[%d]=%d",i,index[i]);
//    }

    com->write(buffer, sizeof (buffer));

    index[0] = (index[0] + 1) % ECG_LEN;
    index[1] = (index[1] + 1) % SPO_LEN;
    index[2] = (index[2] + 1) % IBP_LEN;
}
