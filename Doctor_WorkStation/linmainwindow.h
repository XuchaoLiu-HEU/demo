#ifndef LINMAINWINDOW_H
#define LINMAINWINDOW_H

#include <QMainWindow>
#include <mainwindow.h>

namespace Ui {
class LinMainWindow;
}

class LinMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LinMainWindow(QWidget *parent = 0);
    ~LinMainWindow();

private slots:
    void on_pushButton_clicked();
    void reshow();

private:
    Ui::LinMainWindow *ui;
};

#endif // LINMAINWINDOW_H
