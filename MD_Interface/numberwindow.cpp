#include "numberwindow.h"
#include <QDebug>
#include <QRegExp>

NumberWindow::NumberWindow(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color: black");

    menuLabel = new QLabel();
    menuLabel->setStyleSheet("color : white; background-color : rgb(50, 50, 50); "
                             "font : 63 italic 9pt \"Lucida Sans Typewriter\"");

    numberLabel = new QLabel();
    numberLabel->setStyleSheet("font : 18pt \"Eras Bold ITC\"");

    unitLabel = new QLabel();
    unitLabel->setStyleSheet("font: 9pt \"Eras Bold ITC\"");

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(unitLabel, 0, 1);
    gridLayout->addWidget(numberLabel, 1, 0);
    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 2);
    gridLayout->setColumnStretch(0, 2);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(menuLabel);
    vLayout->addLayout(gridLayout);
    vLayout->setStretch(0, 1);
    vLayout->setStretch(1, 5);
    vLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(vLayout);
    this->setContentsMargins(0, 0, 0, 0);
}

NumberWindow::~NumberWindow()
{
    delete menuLabel;
    delete unitLabel;
    delete numberLabel;
}

void NumberWindow::setDataColor(int r, int g, int b)
{
    QString baseStyle = unitLabel->styleSheet() + ";";
    unitLabel->setStyleSheet(baseStyle + QString("color : rgb(%1, %2, %3)").arg(r).arg(g).arg(b));

    baseStyle = numberLabel->styleSheet() + ";";
    numberLabel->setStyleSheet(baseStyle + QString("color : rgb(%1, %2, %3)").arg(r).arg(g).arg(b));
}

void NumberWindow::setNumberText(QString s)
{
    numberLabel->setText(s);
}

void NumberWindow::setNumberSize(int pt)
{
    QString style_str = numberLabel->styleSheet();
    QRegExp express("[0-9]*pt");

    style_str.replace(express, QString("%1pt").arg(pt));
    numberLabel->setStyleSheet(style_str);
}

void NumberWindow::setUnitText(QString s)
{
    unitLabel->setText(s);
}

void NumberWindow::setMenuText(QString s)
{
    menuLabel->setText(s);
}

void NumberWindow::setUnitVisible(bool flag)
{
    unitLabel->setVisible(flag);
}
