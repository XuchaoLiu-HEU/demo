#include "patientmodel.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPushButton>

PatientModel::PatientModel(){}

bool PatientModel::setName(int id, QString name)
{
    //todo
    bool ok;
    QSqlQuery query;
    query.prepare("update zgmf.patient set 姓名=:name where id = :id");
    query.bindValue(":name",name);
    query.bindValue(":id",id);
    ok = query.exec();
    qDebug()<<id<<name;
    if(!ok)
    {
        qDebug()<<"setname error"<<query.lastError();
    }
    return true;
}

bool PatientModel::setSex(int id, QString sex)
{
    //todo
    bool ok;
    QSqlQuery query;
    query.prepare("update zgmf.patient set 性别=:sex where id = :id");
    query.bindValue(":sex",sex);
    query.bindValue(":id",id);
    ok = query.exec();
    qDebug()<<id<<sex;
    if(!ok)
    {
        qDebug()<<"setname error"<<query.lastError();
    }
    return true;
}

bool PatientModel::setSerial(int id, QString serial)
{
    //todo
    bool ok;
    QSqlQuery query;
    query.prepare("update zgmf.device set 设备编号=:serial where dev_id = :id");
    query.bindValue(":serial",serial);
    query.bindValue(":id",id);
    ok = query.exec();
    qDebug()<<id<<serial;
    if(!ok)
    {
        qDebug()<<"setname error"<<query.lastError();
    }
    return true;
}

bool PatientModel::setData(const QModelIndex &index,const QVariant &value,int)
{
    if (index.column() < 1 || index.column() > 3)
    {
        return false;
    }

    //获取当前行的第0列内容
    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(),0);

    int id = QSqlQueryModel::data(primaryKeyIndex).toInt();

    bool ok;
    if(index.column() == 1)
    {
        // 修改病人姓名
        ok = this->setName(id,value.toString());
    }else if(index.column() == 2)
    {
        // 修改病人性别
        ok = this->setSex(id,value.toString());
    }else if(index.column() == 3)
    {
        // 添加设备
        ok = this->setSerial(id,value.toString());
    }
    if(ok)
    {
        QSqlQuery query;
        int dev_id = 0;
        query.exec("select * from zgmf.patient "
                   "left join device_patient on patient.id = device_patient.patient_id "
                   "left join device on device.dev_id = device_patient.dev_id");
        query.last();
        dev_id = query.value(0).toInt();
        for (int i=0;i<=dev_id;i++)
        {
            query.prepare("update zgmf.device set refresh=now() where dev_id = :dev_id");
            query.bindValue(":dev_id",i);
            query.exec();
        }
        this->setQuery("select id,姓名,性别,设备编号,case when refresh>now()-20 then '在线' else '离线' "
                       "end as '在线状态',入院时间 from patient "
                       "left join device_patient on patient.id = device_patient.patient_id "
                       "left join device on device.dev_id = device_patient.dev_id");
    }

    return ok;
}

