#ifndef ECGWINDOW_H
#define ECGWINDOW_H

#include <QWidget>
#include <serialrx.h>
#include <wavewidget.h>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <stdio.h>
#include <QDebug>
#define DEBUG_FUNC qDebug("Func: %s, Line: %d", __FUNCTION__, __LINE__)

// ecgwindow用于显示三种波形，并对收到的串口数据进行分发

class WaveWindow : public QWidget
{
    Q_OBJECT
public:
    explicit WaveWindow(QWidget *parent = nullptr);
    ~WaveWindow();
    void setLableText(QString s);
    void setWaveColor(int r, int g, int b);
    void setWaveScale(int x);
    void setData(int data, int type);             // 可以在父亲界面完成缩放
signals:
    void sendData(int data, int type);

private slots:
    void on_menuButton_clicked();

private:
    WaveWidget *wave;
    QPushButton *menuButton;
    int baseScale;
    int waveScale;
    int dataIndex;
    QString baseText;

};

#endif // ECGWINDOW_H
