#include "menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color : rgb(50, 50, 50)");

    deviceLabel = new QLabel();
    deviceLabel->setStyleSheet("color : white; background-color : rgb(50, 50, 50); font: italic 13pt \"Calibri\"");
    deviceLabel->setText("Device:D001");

    patientLabel = new QLabel();
    patientLabel->setStyleSheet("color : white; background-color : rgb(50, 50, 50); font: italic 13pt \"Calibri\"");
    patientLabel->setText("patient:--");

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(deviceLabel);
    hlayout->addWidget(patientLabel);
    hlayout->setContentsMargins(0, 0, 0, 0);

    this->setContentsMargins(0, 0, 0, 0);
    this->setLayout(hlayout);
}

MenuWindow::~MenuWindow()
{
    delete deviceLabel;
    delete patientLabel;
}

void MenuWindow::setPatientInfo(QStringList info)
{
    patientLabel->setText("patient:" + info[0] + "," + info[1]);
}
