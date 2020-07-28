#ifndef ECGWIDGET_H
#define ECGWIDGET_H

#include <QWidget>
#include <QPainter>

#include <stdio.h>
#include <QDebug>
#define DEBUG_FUNC qDebug("Func: %s, Line: %d", __FUNCTION__, __LINE__)

// ecg机器波形绘制,需要利用串口，实时获取数据

class WaveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaveWidget(QWidget *parent = nullptr);
    ~WaveWidget() override;
    void setWaveColor(int r, int g, int b);

public slots:
    void getData(int data, int type);
private:
    QPixmap *pixmap;
    QColor color;
    qreal x, y, xold, yold;
    int dataType;

    virtual void paintEvent(QPaintEvent *event) override;

    void paintWave(QPainter *painter);
    void paintOnPixmap();
};

#endif // ECGWIDGET_H
