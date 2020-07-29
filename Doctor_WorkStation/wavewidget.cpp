#include "wavewidget.h"
#include <QtMath>
#include <realdata.h>
#include <mainwindow.h>
#include <QTimer>



WaveWidget::WaveWidget(QWidget *parent, int type) : QWidget(parent)
{
    wtime->start(10);
    connect(wtime,SIGNAL(timerEvent(QTimerEvent*)),this,SLOT(WtimerEvent(QTimerEvent*)));
    // 1、在pixmap上绘制图形，可保证刷新后以前数据不丢失
    pixmap = new QPixmap(2048, 1024);

    pixmap->fill(Qt::black);

    index = 0;          // 标记在界面的位置
    dataIndex = 0;      // 标记数据在数组中的位置

    this->dataType = type;
    switch (type) {
    case 1:
        dataLen = ECG_LEN;
        dataList = MainWindow::ecgw;
        break;
    case 2:
        dataLen = SPO_LEN;
        dataList = MainWindow::spow;;
        break;
    case 3:
        dataLen = IBP_LEN;
        dataList = MainWindow::ibpw;;
        break;
    }

    step = 4;
}

WaveWidget::~WaveWidget()
{
    delete pixmap;
}

void WaveWidget::setWaveColor(int r, int g, int b)
{
    this->color.setRgb(r, g, b);
}

void WaveWidget::setStep(int scale)
{
    this->step = scale;
}


void WaveWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter();
    painter->begin(this);
    painter->resetTransform();
    painter->drawPixmap(0, 0, *pixmap);
    painter->end();

}

void WaveWidget::WtimerEvent(QTimerEvent *)
{
    paintOnPixmap();
    this->update();
}


void WaveWidget::paintWave(QPainter *painter)
{

    // 画笔
    QPen *pen = new QPen();
    pen->setColor(color);
    painter->setPen(*pen);
    // 窗口参数
    int height = this->height();
    int width =  this->width();
    //int step = 3;
    if(index >= width){
        index = 0;
    }
    if(dataIndex >= dataLen){
        dataIndex = 0;
    }

    qreal value = dataList[dataIndex];
    int y = 0;

    switch (dataType) {
    case 1:
        y = round(3 * height / 4 - (value - 2048) / 600 * height / 2);
        break;
    case 2:
        y = round(3 * height / 4 - value / 100 * height / 2);
        break;
    case 3:
        y = round(3 * height / 4 - value / 100 * height / 2);
        break;
    }

    // 擦除当前列
    painter->save();
    pen->setColor(Qt::black);
    painter->setPen(* pen);
    painter->drawLine(index, 0, index, height);
    painter->restore();

    // 绘制数据
    if (index + 1 >= width){
        painter->drawPoint(index, y);
    } else{
        // 下一个数据
        int nextIndex = dataIndex + step;
        if(nextIndex >= dataLen){
            nextIndex = 0;
            //qDebug() << "HERE";
        }
        qreal value1 = dataList[nextIndex];

        int y1 = 0;

        switch (dataType) {
        case 1:
            y1 = round(3 * height / 4 - (value1 - 2048) / 600 * height / 2);
            break;
        case 2:
            y1 = round(3 * height / 4 - value1 / 100 * height / 2);
            break;
        case 3:
            y1 = round(3 * height / 4 - value1 / 100 * height / 2);
            break;
        }

        painter->drawLine(index, y, index + 1, y1);
    }
    // 修改标记
    index += 1;
    dataIndex += step;

}


void WaveWidget::paintOnPixmap()
{
    QPainter cPainter;
    cPainter.begin(this->pixmap);
    cPainter.resetTransform();
    paintWave(&cPainter);
    cPainter.end();
}
