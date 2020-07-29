#include "ecgmainwindow.h"
#include "ui_ecgmainwindow.h"
#include<QWidget>
#include<QDebug>
#include<QPainter>
#include<QBoxLayout>
#include<QLabel>
#include<qmath.h>
#include<QBasicTimer>
#include<QSqlQuery>
#include<mainwindow.h>


EcgMainWindow::EcgMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EcgMainWindow)
{
    ui->setupUi(this);
}

EcgMainWindow::~EcgMainWindow()
{
    delete ui;

}

class QWidgetDraw : public QWidget
{
public:
    int ecgDataIndex;
    int rem = 0;
    qreal index;
    qreal range;
    QPixmap *pixmap;
    QWidgetDraw(QWidget *parent=0):QWidget(parent)
    {
        QBasicTimer *timer = new QBasicTimer();
        timer->start(10,this);

        pixmap = new QPixmap(this->width(),this->height());
        pixmap->fill(Qt::black);

        index = 0;
        range = 0;
        ecgDataIndex = 0;
    }

    virtual void timerEvent(QTimerEvent *event)
    {
        drawWaveToPixmap();
        this->update();
    }

    void drawWaveToPixmap()
    {
        QPainter pixPainter;
        pixPainter.begin(this->pixmap);
        drawSineWave(&pixPainter);
        pixPainter.end();
    }

    virtual void paintEvent(QPaintEvent *event)
    {
        QPainter * painter = new QPainter();
        painter->begin(this);
        painter->resetTransform();//坐标系复位
        painter->setBrush(QColor("#F40002"));
        painter->setPen(QColor("#00FF02"));
        painter->drawPixmap(0,0,*pixmap);
        painter->resetTransform();
        painter->end();
    }

    void drawSineWave(QPainter * painter)
    {
        QPen *pen = new QPen();
        pen->setWidth(2);
        pen->setColor(QColor("#00FF02"));
        painter->setPen(*pen);
        //擦除当前列
        painter->save();
        pen->setColor(Qt::black);
        painter->setPen(*pen);
        painter->drawLine(index,0,index,height());
        painter->restore();

        qreal height = this->height();

        qreal value = MainWindow::ecgw[ecgDataIndex];
        int y = round(height/2 - ((value-2048)/600)*height/2);
        painter->drawPoint(index,y);
        if (index != 0)
            painter->drawLine(index-1,rem,index,y);
        rem = y;

        index = index + 1;
        if(index > width())
            index = 0;
        ecgDataIndex = ecgDataIndex + 5;

        if(ecgDataIndex >= sizeof(MainWindow::ecgw)/sizeof(int))
            ecgDataIndex = 0;
    }
};

class EcgWave: public QWidget
{
public:
    EcgWave(QWidget *parent = 0):QWidget(parent)
    {
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setStyleSheet("background-color:black");

        QLabel *title = new QLabel("ECG");
        title->setStyleSheet("color:white");
        QWidgetDraw *waveWin=new QWidgetDraw();
        QVBoxLayout* layout = new QVBoxLayout();

        layout->addWidget(title);
        layout->addWidget(waveWin);
        layout->setStretch(0,1);
        layout->setStretch(1,5);
        this->setLayout(layout);
    }
};

bool EcgMainWindow::connectDB()
{

    QWidget *widgetMainTitle = new QWidget();
    widgetMainTitle->setStyleSheet("background-color:rgb(100,100,100)");

    EcgWave *wave = new EcgWave();
    wave->setStyleSheet("background-color:black");
    EcgWave *widgetWavbeSpo2 = new EcgWave();
    widgetWavbeSpo2->setStyleSheet("background-color:black");
    EcgWave *widgetWaveResp = new EcgWave();
    widgetWaveResp->setStyleSheet("background-color:black");

    QVBoxLayout *layoutWave = new QVBoxLayout();
    layoutWave->addWidget(wave);
    layoutWave->addWidget(widgetWavbeSpo2);
    layoutWave->addWidget(widgetWaveResp);
    layoutWave->setStretch(0,1);
    layoutWave->setStretch(1,1);
    layoutWave->setStretch(2,1);

    QWidget *widgetHr = new QWidget();
    widgetHr->setStyleSheet("background-color:rgb(100,100,100)");
    QWidget *widgetSt = new QWidget();
    widgetSt->setStyleSheet("background-color:rgb(100,100,200)");
    QWidget *widgetNibp = new QWidget();
    widgetNibp->setStyleSheet("background-color:rgb(100,200,100)");
    QWidget *widgetSpo2 = new QWidget();
    widgetSpo2->setStyleSheet("background-color:rgb(200,100,100)");
    QWidget *widgetTemp = new QWidget();
    widgetTemp->setStyleSheet("background-color:rgb(200,200,100)");
    QWidget *widgetResp= new QWidget();
    widgetResp->setStyleSheet("background-color:rgb(100,200,200)");
    QWidget *widgetCo2 = new QWidget();
    widgetCo2->setStyleSheet("background-color:rgb(200,100,200)");

    QHBoxLayout *layoutTableLine1 = new QHBoxLayout();
    layoutTableLine1->addWidget(widgetHr);
    layoutTableLine1->addWidget(widgetSt);

    QHBoxLayout *layoutTableLine2 = new QHBoxLayout();
    layoutTableLine2->addWidget(widgetNibp);

    QHBoxLayout *layoutTableLine3 = new QHBoxLayout();
    layoutTableLine3->addWidget(widgetSpo2);
    layoutTableLine3->addWidget(widgetTemp);

    QHBoxLayout *layoutTableLine4 = new QHBoxLayout();
    layoutTableLine4->addWidget(widgetResp);
    layoutTableLine4->addWidget(widgetCo2);

    QVBoxLayout *layoutTable = new QVBoxLayout();
    layoutTable->addLayout(layoutTableLine1);
    layoutTable->addLayout(layoutTableLine2);
    layoutTable->addLayout(layoutTableLine3);
    layoutTable->addLayout(layoutTableLine4);

    QHBoxLayout *layoutDown = new QHBoxLayout();
    layoutDown->addLayout(layoutWave);
    layoutDown->addLayout(layoutTable);
    layoutDown->setStretch(0,2);
    layoutDown->setStretch(1,1);

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(widgetMainTitle);
    layoutMain->addLayout(layoutDown);
    layoutMain->setStretch(0,1);
    layoutMain->setStretch(1,10);

    QWidget *widgetMain = new QWidget();
    widgetMain->resize(800,480);
    widgetMain->setWindowFlags (Qt::FramelessWindowHint);
    widgetMain->setLayout(layoutMain);

    widgetMain->setParent(this);

    return true;


}
