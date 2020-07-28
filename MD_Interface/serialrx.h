#ifndef SERIALRX_H
#define SERIALRX_H

#include <QObject>
#include <QSerialPort>

class SerialRx : public QObject
{
    Q_OBJECT
public:
    explicit SerialRx(QString portName = "com2",QObject *parent = nullptr);

signals:
    void rxReadFinish(int ecg, int spo, int ibp);
    void heartRateReadFinish(int hr);

private slots:
    void serialReadReady();

private:
    QSerialPort *com;
    int data[3];

    // 计算心率
    int ecgBuffer[2000];
    int ecgBufferIndex;
    int ecgPulseMax;
    int ecgPulseMid;
    bool pulse;
    bool prePulse;
    qint64 pulseA, pulseB;      // 前、后两次脉冲的时刻值

    bool bccCheck(char *buffer, int len);
    void rxHandleData(char *buffer);
};

#endif // SERIALRX_H
