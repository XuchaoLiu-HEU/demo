#include "timewindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPushButton>
#include <QString>

Timewindow::Timewindow(){}

bool Timewindow::settime(int id,QString time)
{
    //todo
    bool ok;
    QSqlQuery query;
    query.prepare("select time,dev_id from zgmf.ecg_sample where time=:time and dev_id = :id");
    query.bindValue(":time",time);
    query.bindValue(":id",id);
    ok = query.exec();
    if(!ok)
    {
        qDebug()<<"setname error"<<query.lastError();
    }
    return true;
}

bool Timewindow::setData(const QModelIndex &index,const QVariant &value,int)
{
    if (index.column() != 0)
    {
        return false;
    }

    //获取当前行的第0列内容
    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(),1);

    int id = QSqlQueryModel::data(primaryKeyIndex).toInt();

    bool ok;
    if(index.column() == 0)
    {
        ok = this->settime(id,value.toString());
    }
    if(ok)
    {
        this->setQuery("select time from zgmf.ecg_sample  where dev_id = "+ QString::number(id));
    }

    return ok;
}
