#ifndef NUMBERWINDOW_H
#define NUMBERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QGridLayout>
#include <QVBoxLayout>

class NumberWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NumberWindow(QWidget *parent = nullptr);
    ~NumberWindow();
    void setDataColor(int r, int g, int b);
    void setNumberText(QString s);
    void setNumberSize(int pt);
    void setUnitText(QString s);
    void setMenuText(QString s);
    void setUnitVisible(bool flag);

signals:

private:
    QLabel *menuLabel;              // 标题头
    QLabel *numberLabel;    // 可以显示多行
    QLabel *unitLabel;              // 显示单位
};

#endif // NUMBERWINDOW_H
