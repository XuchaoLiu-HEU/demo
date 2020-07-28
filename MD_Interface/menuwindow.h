#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

    void setPatientInfo(QStringList info);
signals:

private:
    QLabel *deviceLabel;
    QLabel *patientLabel;
};

#endif // MENUWINDOW_H
