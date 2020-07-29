#ifndef ECGMAINWINDOW_H
#define ECGMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class EcgMainWindow;
}

class EcgMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EcgMainWindow(QWidget *parent = 0);
    ~EcgMainWindow();
    bool connectDB();


private:
    Ui::EcgMainWindow *ui;
};

#endif // ECGMAINWINDOW_H
