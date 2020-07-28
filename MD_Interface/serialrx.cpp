#include "serialrx.h"
#include <QDebug>
#include <QDateTime>

#define ecgPulseZero 2048

SerialRx::SerialRx(QString portName, QObject *parent) : QObject(parent)
{
    ecgBufferIndex = 0;
    ecgPulseMax = 2048;
    ecgPulseMid = 2048;
    pulse = false;
    prePulse = false;
    pulseA = 0;
    pulseB = 0;

    com = new QSerialPort();
    com->setPortName(portName);

    if (!com->open(QSerialPort::ReadWrite)) {
        qDebug() << portName << "打开失败";
        return;
    }

    qDebug() << portName << "打开成功";
    com->setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
    com->setStopBits(QSerialPort::OneStop);

    connect(com, SIGNAL(readyRead()), this, SLOT(serialReadReady()));
}

void SerialRx::serialReadReady()
{
    //qDebug("receiving...");

    int len = 0;
    char buffer[7];
    while (!com->atEnd()) {
        // 帧同步
        len = int(com->read(buffer, 1));
        if (buffer[0] != 0x08) {
            continue;
        }

        // 读取数据
        len += com->read(buffer + 1, 6);

        // 校验、处理数据
        if (bccCheck(buffer, len)){
            rxHandleData(buffer);
        } else {
            qDebug("数据未通过校验");
        }
    }
}

bool SerialRx::bccCheck(char *buffer, int len)
{

    // 检查数据长度，检查包头最高位是否正确
    if (len != 7 || buffer[0] != 0x08) {
        return false;
    }

    // 检查字节最高位
    for (int i = 1; i < len - 1; i++){
        if ((buffer[i] & 0x80) != 0x80){
            return false;
        }
    }

    // bcc校验
    char bcc = 0x00;
    for (int i = 0; i < len - 1; i++) {
        bcc ^= buffer[i];
    }
    if (bcc == buffer[len - 1]) {
        return true;
    } else {
        return false;
    }
}

void SerialRx::rxHandleData(char *buffer)
{
    // 解析16进制数据
    // 帧结构,buffer[0]=0x08，buffer[1]数据头,buffer[2]、buffer[3]为ecg数据高、低位
    // buffer[4]为spo数据，buffer[5]为ibp数据
    // ecg
    int ecgH = ((buffer[1] & 0x40) << 1) | (buffer[2] & 0x7F);
    int ecgL = ((buffer[1] & 0x20) << 2) | (buffer[3] & 0x7F);
    data[0] = (ecgH << 8) | ecgL;
    // spo
    data[1] = ((buffer[1] & 0x10) << 3) | (buffer[4] & 0x7F);
    // ibp
    data[2] = ((buffer[1] & 0x08) << 4) | (buffer[5] & 0x7F);

    emit rxReadFinish(data[0], data[1], data[2]);

    // 计算心率
    ecgBuffer[ecgBufferIndex] = data[0];
    ecgBufferIndex += 1;

    if (data[0] > ecgPulseMax) {
        ecgPulseMax = data[0];
    }

    if (ecgBufferIndex >= 2000) {
        ecgBufferIndex = 0;
        ecgPulseMid = (ecgPulseMax + ecgPulseZero) / 2;
    }

    prePulse = pulse;
    if (data[0] > ecgPulseMid) {
        pulse = 1;
    } else {
        pulse = 0;
    }

    if (!prePulse && pulse) {
        pulseA = pulseB;
        pulseB = QDateTime::currentMSecsSinceEpoch();
        int ecgPeriod = int(pulseB - pulseA);
        int hr = int(60 * 1000 * 5/ ecgPeriod);        // 在发送端，将速度降低了5倍，此处用算法弥补
        emit heartRateReadFinish(hr);
    }
}

