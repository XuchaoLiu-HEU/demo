#include "wavewindow.h"


WaveWindow::WaveWindow(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: black");

    wave = new WaveWidget(this);
    connect(this, &WaveWindow::sendData, wave, &WaveWidget::getData);

    menuButton = new QPushButton();
    menuButton->setStyleSheet("color: white; font: 63 italic 11pt \"Lucida Sans Typewriter\"; text-align: left");
    connect(menuButton, &QPushButton::clicked, this, &WaveWindow::on_menuButton_clicked);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(menuButton);
    vlayout->addWidget(wave);
    vlayout->setStretch(0, 1);
    vlayout->setStretch(1, 10);
    this->setLayout(vlayout);

    waveScale = 1;
    dataIndex = 0;
    baseScale = 4;
}

WaveWindow::~WaveWindow()
{
    delete wave;
    delete menuButton;
}

void WaveWindow::setLableText(QString s)
{
    QString s1 = QString::asprintf("%d.0", waveScale);
    baseText = s;
    if (s.length() == 3){
        baseText += " ";
    }
    menuButton->setText(baseText + "           X" + s1);
}

void WaveWindow::setWaveColor(int r, int g, int b)
{
    wave->setWaveColor(r, g, b);
}

void WaveWindow::setWaveScale(int x)
{
    this->waveScale = x;

    QString s1 = QString::asprintf("%d.0", waveScale);
    menuButton->setText(baseText + "           X" + s1);
}

void WaveWindow::setData(int data, int type)
{
    if (dataIndex < baseScale / waveScale){
        dataIndex += 1;
    } else {
        // dataIndex == data
        emit sendData(data, type);
        dataIndex = 0;
    }

}

void WaveWindow::on_menuButton_clicked()
{
    // 实现1、2、4倍放缩
    waveScale *= 2;
    if (waveScale > baseScale) {
        waveScale = 1;
    }

    QString s1 = QString::asprintf("%d.0", waveScale);
    menuButton->setText(baseText + "           X" + s1);
}

