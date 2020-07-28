#include "wavewidget.h"
#include <QtMath>


WaveWidget::WaveWidget(QWidget *parent) : QWidget(parent)
{
    // 1、在pixmap上绘制图形，可保证刷新后以前数据不丢失
    pixmap = new QPixmap(2048, 1024);

    pixmap->fill(Qt::black);
    // 初始化数据点
    x = 0;
    y = 0;
    xold = 0;
    yold = 0;
    // 初始化数据类型
    dataType = -1;
}

WaveWidget::~WaveWidget()
{
    delete pixmap;
}

void WaveWidget::setWaveColor(int r, int g, int b)
{
    this->color.setRgb(r, g, b);
}

void WaveWidget::getData(int data, int type)
{
    y = data;
    dataType = type;
    paintOnPixmap();
    this->update();
}

void WaveWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter();
    painter->begin(this);
    painter->resetTransform();
    painter->drawPixmap(0, 0, *pixmap);
    painter->end();

}


void WaveWidget::paintWave(QPainter *painter)
{
    int width = this->width();
    int height = this->height();

    if (x >= width){
        x = 0;
    }

    QPen *pen = new QPen();
    pen->setColor(color);
    painter->setPen(*pen);

    // 擦除以前绘制痕迹
    painter->save();
    painter->setPen(QColor(0, 0, 0));
    painter->drawLine(int(x), 0, int(x), height);
    painter->restore();

    //qDebug() << "dataType = " << dataType << ";before = " << y;

    switch (dataType) {
    case 1:
        // ecg
        y = round(3 * height / 4 - (y - 2048) / 600 * height / 2);
        break;
    case 2:
        // spo, 加大向下偏移量
        y = round(3 * height / 4 - y / 100 * height / 2);
        break;
    case 3:
        // ibp
        y = round(3 * height / 4 - y / 100 * height / 2);
        break;
    }

    //qDebug() << "dataType = " << dataType << ";after = " << y;

    // 绘制线
    if (int(x) == 0){
        // 当前点是最开始的点
        painter->drawPoint(QPointF(x, y));
    } else {
        painter->drawLine(QPointF(xold, yold), QPointF(x, y));
    }

    xold = x;
    yold = y;
    x += 1;
}

void WaveWidget::paintOnPixmap()
{
    QPainter cPainter;
    cPainter.begin(this->pixmap);
    cPainter.resetTransform();
    paintWave(&cPainter);
    cPainter.end();
}
