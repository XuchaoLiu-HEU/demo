#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QTableView>
#include <patientmodel.h>
#include <mainwindow.h>
#include <mydelegate.h>
#include <linmainwindow.h>
#include <ecgmainwindow.h>
#include <QMessageBox>
#include <wavewindow.h>
#include <QHostAddress>
#include <timewindow.h>

int MainWindow::ecgw[500]={2048};
int MainWindow::spow[500]={0};
int MainWindow::ibpw[500]={0};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(tcpReadyRead()));

    mSocket->connectToHost(QHostAddress::LocalHost,1234);//连接到本地回环的1234端口
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tcpReadyRead()
{
    qDebug()<<"我来了";
    QString result = QString::fromLocal8Bit(mSocket->readAll());
    if(result.mid(0,3) == "ECG" && mSocket)
    {
        QStringList dataList = result.split("^");
        QStringList ecg = dataList[1].split(",");
        QStringList spo =dataList[2].split(",");
        QStringList ibp =dataList[3].split(",");
        short ecgwaves[ecg.size()-1];
        short spowaves[spo.size()-1];
        short ibpwaves[ibp.size()];
        for(int i=0;i<ecg.size()-1;i++)
        {
             ecgwaves[i]=ecg.at(i).toShort();          //数组赋值
             ecgw[i] = (int)ecgwaves[i];
        }
        for(int i=0;i<spo.size()-1;i++)
        {
             spowaves[i]=spo.at(i).toShort();          //数组赋值
             spow[i] = (int)spowaves[i];
        }
        for(int i=0;i<ibp.size();i++)
        {
             ibpwaves[i]=ibp.at(i).toShort();          //数组赋值
             ibpw[i] = (int)ibpwaves[i];
        }
        qDebug()<<"我走了";
        WaveWindow *w1=new WaveWindow(nullptr,1);
        WaveWindow *w2=new WaveWindow(nullptr,2);
        WaveWindow *w3=new WaveWindow(nullptr,3);
        w1->show();
        w2->show();
        w3->show();
    }else
    {
        QString message = "Bad request";
        mSocket->write(message.toLatin1());
    }
}


bool MainWindow::connectDB()
{ 

    //更新refresh
    bool ok;
    int dev_id = 0;
    QSqlQuery query;
    query.exec("select * from zgmf.patient");
    query.last();
    dev_id = query.value(0).toInt();
    for (int i=0;i<=dev_id;i++)
    {
        QSqlQuery queryRefresh;
        queryRefresh.prepare("update zgmf.device set refresh=now() where dev_id = :dev_id");
        queryRefresh.bindValue(":dev_id",i);
        ok = queryRefresh.exec();
        if(!ok)
        {
            qDebug()<<"设备在线状态更新失败"<<queryRefresh.lastError();
        }
    }


    patientsDev.setQuery("select id,姓名,性别,设备编号,case when refresh>now()-20 then '在线' else '离线' "
                         "end as '在线状态',入院时间 from patient "
                         "left join device_patient on patient.id = device_patient.patient_id "
                         "left join device on device.dev_id = device_patient.dev_id");

    patientstime.setQuery("select time from zgmf.ecg_sample  where now()-time = "
                          "(select min(now()-time) from zgmf.ecg_sample)");


    ui->tableView->setModel(&patientsDev);
    ui->tableView_2->setModel(&patientstime);
    ui->tableView->setAlternatingRowColors(true);//表格颜色隔行区分
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);//最后一列占满剩余空间
    ui->tableView->setItemDelegateForColumn(2,new MyDelegate(this));//下拉列表
    ui->tableView->setItemDelegateForColumn(3,new MyDelegate(this));
    ui->tableView_2->setItemDelegateForColumn(0,new MyDelegate(this));
    ui->tableView_2->horizontalHeader()->hide();
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);


    return true;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query;
    bool ok;
    int dev_id = 0;
    query.exec("insert into zgmf.patient (姓名,性别) values (null,null);"
               "insert into zgmf.device (设备编号,refresh) values (null,null);"
               "insert into zgmf.device_patient (dev_id,patient_id) values "
               "((select max(dev_id) from device),(select max(id) from patient));");

    query.exec("select * from zgmf.patient "
               "left join device_patient on patient.id = device_patient.patient_id "
               "left join device on device.dev_id = device_patient.dev_id");
    query.last();
    dev_id = query.value(0).toInt();
    for (int i=0;i<=dev_id;i++)
    {
        query.prepare("update zgmf.device set refresh=now() where dev_id = :dev_id");
        query.bindValue(":dev_id",i);
        ok = query.exec();
        if(!ok)
        {
            qDebug()<<"设备在线状态更新失败"<<query.lastError();
        }
    }

    patientsDev.setQuery("select id,姓名,性别,设备编号,case when refresh>now()-20 then '在线' else '离线' "
                         "end as '在线状态',入院时间 from patient "
                         "left join device_patient on patient.id = device_patient.patient_id "
                         "left join device on device.dev_id = device_patient.dev_id");
}

void MainWindow::on_pushButton_2_clicked()
{
    emit sendsignal();
    this->close();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString id;
    QString time;
    id = ui->lineEdit->text();
    QSqlQuery query;
    query.exec("select * from zgmf.patient");
    query.last();
    int dev_id = query.value(0).toInt();
    if(id == "")
        QMessageBox::warning(this,tr("提示"),"id不能为空！");
    else if(id.toInt()>dev_id)
        QMessageBox::warning(this,tr("提示"),"找不到该患者！");
    else
    {
        QSqlQuery querytime;
        querytime.exec("select time from zgmf.ecg_sample  where now()-time = "
                   "(select min(now()-time) from zgmf.ecg_sample)");
        if(querytime.next())
        {
            time = querytime.value(0).toString();
        }
        query.prepare("SELECT * FROM ecg_sample "
                      "WHERE time=:time and dev_id=:dev_id");
        query.bindValue(":time", time);
        query.bindValue(":dev_id", id);

        WaveWindow *w=new WaveWindow(nullptr,1);

        if(query.exec() && query.next())
        {
            QByteArray ecgwave = query.value("ecg").toByteArray();
            QByteArray spowave = query.value("spo").toByteArray();
            QByteArray ibpwave = query.value("ibp").toByteArray();
            for (int i=0;i<ecgwave.size()/2;i++)
            {
                ecgw[i] = (int)(((ecgwave.at(2*i+1)+1)<<8)+ecgwave.at(2*i));
                spow[i] = (int)(((spowave.at(2*i+1)+1)<<8)+spowave.at(2*i));
                ibpw[i] = (int)(((ibpwave.at(2*i+1)+1)<<8)+ibpwave.at(2*i));
            }
            w->setLableText("ECG");
            w->setWaveColor(255,0,0);
            w->show();
        }else
        {
            QMessageBox::warning(this,tr("提示"),"无记录！");
        }

    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(mSocket)
    {
        QString id;
        id = ui->lineEdit_2->text();
        QSqlQuery query;
        query.exec("select * from zgmf.patient");
        query.last();
        int dev_id = query.value(0).toInt();
        if(id == "")
            QMessageBox::warning(this,tr("提示"),"id不能为空！");
        else if(id.toInt()>dev_id)
            QMessageBox::warning(this,tr("提示"),"找不到该患者！");
        else
        {
            QString wave = "wave";
            mSocket->write(wave.toLatin1());
        }
    }
}
