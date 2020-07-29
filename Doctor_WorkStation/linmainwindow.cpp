#include "linmainwindow.h"
#include "ui_linmainwindow.h"
#include <mainwindow.h>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>

LinMainWindow::LinMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LinMainWindow)
{
    ui->setupUi(this);
}

LinMainWindow::~LinMainWindow()
{
    delete ui;
}

void LinMainWindow::reshow()
{
    this->show();
}

void LinMainWindow::on_pushButton_clicked()
{
    QString user;
    QString pwd;
    user = ui->lineEdit_2->text();
    pwd = ui->lineEdit->text();
    if(user == "")
        QMessageBox::warning(this,tr("提示"),"用户名不能为空！");
    else if(pwd == "")
        QMessageBox::warning(this,tr("提示"),"密码不能为空！");
    else if (user != "zg")
        QMessageBox::warning(this, tr("提示"), tr("用户名不存在，请重新输入！"), QMessageBox::Yes);
    else if(user == "zg" && pwd != "123456")
        QMessageBox::warning(this, tr("提示"), tr("用户名密码不匹配，请重新输入!"), QMessageBox::Yes);
    else if(user == "zg" && pwd == "123456")
    {
        //加载驱动
        QSqlDatabase db;
        if(QSqlDatabase::contains("qt_sql_default_connection"))
          db = QSqlDatabase::database("qt_sql_default_connection");
        else
          db = QSqlDatabase::addDatabase("QMYSQL");

        //配置服务器地址
        db.setHostName("localhost");

        //数据库名字
        db.setDatabaseName("zgmf");

        //用户名密码
        db.setUserName("zg");
        db.setPassword("123456");

        //打开链接
        //要判断登录是否成功
        bool dbOk = db.open();
        if(!dbOk)
        {
            QMessageBox::warning(this,tr("提示"),"登录失败！", QMessageBox::Yes);
        }else
        {
            this->hide();
            MainWindow *w = new MainWindow;
            connect(w,SIGNAL(sendsignal()),this,SLOT(reshow()));
            w->connectDB();
            w->show();
        }
    }
}


