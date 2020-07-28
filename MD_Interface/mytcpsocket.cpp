#include "mytcpsocket.h"
#include <QDebug>


MyTcpSocket::MyTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::myReadyRead);
}

void MyTcpSocket::myReadyRead()
{
    // 在子线程处理数据
    QString clientRequest(this->readAll());
    QStringList dataList(clientRequest.split("^"));
    qDebug() << dataList;

    if (dataList[0] == "wave"){
        QString response = "ECG^";
        for (int i = 0; i < ECG_LEN; i++) {
            response.append(QString::number(Ecg_Data[i]) + ",");
        }
        response.chop(1);       // 删除,

        response.append("\nSPO^");
        for (int i = 0; i < SPO_LEN; i++) {
            response.append(QString::number(Spo_Data[i]) + ",");
        }
        response.chop(1);

        response.append("\nIBP^");
        for (int i = 0; i < IBP_LEN; i++) {
            response.append(QString::number(Ibp_Data[i]) + ",");
        }
        response.chop(1);

        this->write(response.toUtf8());

    } else if (dataList[0] == "patient") {
        QStringList patientInfo(dataList[1].split(","));
        emit patientReadFinish(patientInfo);
        qDebug() << patientInfo;
    }

}
