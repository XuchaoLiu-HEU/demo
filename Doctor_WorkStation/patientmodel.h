#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QDebug>
#include<QSqlError>
#include<QDateTime>
#include<QSqlQueryModel>
#include<QTableView>


class PatientModel : public QSqlQueryModel
{
public:
    PatientModel();
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        Qt::ItemFlags flags = QSqlQueryModel::flags(index);

        if(index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 3)
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
    bool setName(int id, QString name);
    bool setSex(int id, QString Sex);
    bool setSerial(int id, QString serial);
    bool setData(const QModelIndex &index,const QVariant &value,int);
};

#endif // PATIENTMODEL_H
