#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: black");

    // 串口
    rx = new SerialRx();
    connect(rx, &SerialRx::rxReadFinish, this, &MainWindow::transitData);
    connect(rx, &SerialRx::heartRateReadFinish, this, &MainWindow::setHeartRate);

    // TCP
    mServer = new QTcpServer();
    mServer->setMaxPendingConnections(2);       // 最大连接数
    connect(mServer, &QTcpServer::newConnection, this, &MainWindow::tcpNewConnection);
    mServer->listen(QHostAddress::Any, 8000);   // 开始监听

    // 界面
    // 左侧
    ecgWave = new WaveWindow();
    ecgWave->setLableText("ECG");
    ecgWave->setWaveColor(0, 255, 0);
    ecgWave->setWaveScale(2);

    spoWave = new WaveWindow();
    spoWave->setLableText("SPO2");
    spoWave->setWaveColor(255, 0, 0);
    spoWave->setWaveScale(4);

    ibpWave = new WaveWindow();
    ibpWave->setLableText("IBP");
    ibpWave->setWaveColor(255, 255, 0);

    QVBoxLayout *vlayout1 = new QVBoxLayout();
    vlayout1->addWidget(ecgWave);
    vlayout1->addWidget(spoWave);
    vlayout1->addWidget(ibpWave);

    // 右侧第一行
    hrWindow = new NumberWindow();
    hrWindow->setMenuText("HR");
    hrWindow->setUnitText("bpm");
    hrWindow->setDataColor(0, 255, 0);
    hrWindow->setNumberText("-?-");

    stWindow = new NumberWindow();
    stWindow->setMenuText("ST");
    stWindow->setUnitVisible(false);
    stWindow->setDataColor(0, 255, 0);
    stWindow->setNumberText("ST1 -?-\nST2 -?-\nPVCs -?-");
    stWindow->setNumberSize(12);

    QHBoxLayout *hlayout1 = new QHBoxLayout();
    hlayout1->addWidget(hrWindow);
    hlayout1->addWidget(stWindow);

    // 右侧第二行
    nibpWindow = new NumberWindow();
    nibpWindow->setMenuText("NIBP");
    nibpWindow->setUnitText("mmHg");
    nibpWindow->setDataColor(0, 0, 255);
    nibpWindow->setNumberText("-?-/-?-    -?-");

    // 右侧第三行
    spoWindow = new NumberWindow();
    spoWindow->setMenuText("SpO2");
    spoWindow->setUnitText("%");
    spoWindow->setDataColor(255, 0, 0);
    spoWindow->setNumberText("-?-");

    tempWindow = new NumberWindow();
    tempWindow->setMenuText("TEMP");
    tempWindow->setUnitText("°C");
    tempWindow->setDataColor(255, 0, 255);
    tempWindow->setNumberText("T1 -?-\nT2 -?-");

    QHBoxLayout *hlayout2 = new QHBoxLayout();
    hlayout2->addWidget(spoWindow);
    hlayout2->addWidget(tempWindow);

    // 右侧第四行
    respWindow = new NumberWindow();
    respWindow->setMenuText("RESP");
    respWindow->setUnitText("bpm");
    respWindow->setDataColor(255, 255, 0);
    respWindow->setNumberText("-?-");

    co2Window = new NumberWindow();
    co2Window->setMenuText("CO2");
    co2Window->setUnitText("mmHg");
    co2Window->setDataColor(255, 255, 0);
    co2Window->setNumberText("-?-");

    QHBoxLayout *hlayout3 = new QHBoxLayout();
    hlayout3->addWidget(respWindow);
    hlayout3->addWidget(co2Window);

    // 右侧
    QVBoxLayout *vlayout2 = new QVBoxLayout();
    vlayout2->addLayout(hlayout1, 1);
    vlayout2->addWidget(nibpWindow, 1);
    vlayout2->addLayout(hlayout2, 1);
    vlayout2->addLayout(hlayout3, 1);

    // 左侧 + 右侧(下部)
    QHBoxLayout *hlayout4 = new QHBoxLayout();
    hlayout4->addLayout(vlayout1, 3);
    hlayout4->addLayout(vlayout2, 1);

    menuWindow = new MenuWindow();
    QVBoxLayout *vlayout3 = new QVBoxLayout();
    vlayout3->addWidget(menuWindow);
    vlayout3->addLayout(hlayout4);

    this->centralWidget()->setLayout(vlayout3);
    this->setContentsMargins(0, 0, 0, 0);
}

MainWindow::~MainWindow()
{
    delete ecgWave;
    delete spoWave;
    delete ibpWave;

    delete menuWindow;

    delete hrWindow;
    delete stWindow;
    delete nibpWindow;
    delete spoWindow;
    delete tempWindow;
    delete respWindow;
    delete co2Window;

    delete rx;
    delete mServer;

    delete ui;
}

void MainWindow::transitData(int ecg, int spo, int ibp)
{    
    ecgWave->setData(ecg, 1);
    spoWave->setData(spo, 2);
    ibpWave->setData(ibp, 3);
}

void MainWindow::setHeartRate(int hr)
{
    hrWindow->setNumberText(QString::number(hr));
}


void MainWindow::tcpNewConnection()
{
    // 监听到tcp请求, mainSocket后期可改为非全局变量
    mainSocket = mServer->nextPendingConnection();
    if (mainSocket) {
        // 传递
        MyTcpSocket *threadSocket = new MyTcpSocket();
        threadSocket->setSocketDescriptor(mainSocket->socketDescriptor());

        // 连接开始就发送获取请求
        threadSocket->write(QString("patient").toUtf8());

        // 开启子线程
        QThread *childThread = new QThread();
        threadSocket->moveToThread(childThread);
        connect(threadSocket, &MyTcpSocket::patientReadFinish, this, &MainWindow::handleTcpData);
        connect(threadSocket, &MyTcpSocket::disconnected, childThread, &QThread::quit);
        childThread->start();
    }
}

void MainWindow::handleTcpData(QStringList info)
{
    menuWindow->setPatientInfo(info);
}















