#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QDebug>
#include<QSqlError>
#include<QDateTime>
#include<QSqlQueryModel>
#include<QTableView>

class Timewindow : public QSqlQueryModel
{
public:
    Timewindow();
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QSqlQueryModel::flags(index);

        if(index.column() == 0)
        {
            flags |= Qt::ItemIsEditable;// | 表示按位或运算
        }
        return flags;
    }
    //单元格居中
    QVariant data(const QModelIndex &index, int role) const override
    {
        QVariant value = QSqlQueryModel::data(index, role);
        if (Qt::TextAlignmentRole == role) {
            value = Qt::AlignCenter;
        }
        return value;
    }
    bool settime(int id, QString time);
    bool setData(const QModelIndex &index,const QVariant &value,int);
};

#endif // TIMEWINDOW_H
