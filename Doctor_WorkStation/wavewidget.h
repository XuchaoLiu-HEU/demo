#ifndef ECGWIDGET_H
#define ECGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include <realdata.h>
#include <QTimer>
#include <stdio.h>
#include <QDebug>
#define DEBUG_FUNC qDebug("Func: %s, Line: %d", __FUNCTION__, __LINE__)

// ecg机器波形绘制,需要利用串口，实时获取数据

class WaveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaveWidget(QWidget *parent = nullptr, int type = 1);
    ~WaveWidget() override;
    void setWaveColor(int r, int g, int b);
    void setStep(int scale);


private:
    QPixmap *pixmap;
    QColor color;
    int index, dataIndex, dataType, step;

    int *dataList, dataLen;

    virtual void paintEvent(QPaintEvent *event) override;

    void paintWave(QPainter *painter);
    void paintOnPixmap();
    QTimer *wtime;

private slots:
    void WtimerEvent(QTimerEvent *);
};

#endif // ECGWIDGET_H
