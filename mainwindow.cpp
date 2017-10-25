#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include<QPainter>
#include<QDesktopWidget>
#include<QScreen>
#include<QMessageBox>
#include<QMetaEnum>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // ui->setupUi(this);
    //setFixedSize(645,414);//设置窗口固定大小
    //setWindowState(Qt::WindowMaximized);
    startInit();
    ui->statusBar_2->setText(tr("串口关闭"));
    setGeometry(400,250,542,390);
    dataTimer=new QTimer(this);
    connect(dataTimer,SIGNAL(timeout()),this,SLOT(readtimeDataSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::startInit()
{
    ui->StartUartbtn->setEnabled(true);
    ui->StopUartbtn->setEnabled(false);
    //初始化读取定时器计数间隔
    timerdly=TIMER_INTERVAL;
    //设置读取计时器
    readTImerA=new QTimer(this);
    connect(readTImerA,SIGNAL(timeout()),this,SLOT(readMycomA()));
}
void MainWindow::setComboBoxEnabled(bool status)        //设置ComboBox控件的可用性
{
    ui->portNameComboBox->setEnabled(status);
    ui->baudRateComboBox->setEnabled(status);
    ui->dataBitsComboBox->setEnabled(status);
    ui->parityComboBox->setEnabled(status);
    ui->stopBitsComboBox->setEnabled(status);

}

void MainWindow::on_StopUartbtn_clicked()        //关闭串口
{
    myComA->close();
    delete myComA;
    dataTimer->stop();
    readTImerA->stop();
    ui->statusBar_2->setText(tr("串口关闭"));
    setComboBoxEnabled(true);
    ui->StartUartbtn->setEnabled(true);
    ui->StopUartbtn->setEnabled(false);

}

void MainWindow::on_StartUartbtn_clicked()      //打开串口
{
    QString portName=ui->portNameComboBox->currentText();//获取串口名
    myComA=new Win_QextSerialPort(portName,QextSerialBase::Polling);//polling为查询方式，EventDriven为事件驱动
    //这里QextSerialBase::QueryMode应该使用QextSerialBase::polling
    if(myComA->open(QIODevice::ReadOnly))
    {
        ui->statusBar_2->setText(tr("串口打开成功"));

    }
    else
    {
         ui->statusBar_2->setText(tr("串口打开失败"));
         return;
    }
    //设置波特率
   // qDebug()<<ui->baudRateComboBoxGPS->currentText();
   // myGpsCom->setBaudRate((BaudRateType)ui->baudRateComboBoxGPS->currentIndex());
    if(ui->baudRateComboBox->currentText()==tr("9600"))
       myComA->setBaudRate(BAUD9600);
    if(ui->baudRateComboBox->currentText()==tr("38400"))
       myComA->setBaudRate(BAUD38400);
    if(ui->baudRateComboBox->currentText()==tr("1152000"))
       myComA->setBaudRate(BAUD115200);
    //设置数据位
    // myComA->setDataBits((DataBitsType)ui->dataBitsComboBox->currentIndex());
    if(ui->dataBitsComboBox->currentText()==tr("5"))
        myComA->setDataBits(DATA_5);
    if(ui->dataBitsComboBox->currentText()==tr("6"))
        myComA->setDataBits(DATA_6);
    if(ui->dataBitsComboBox->currentText()==tr("7"))
        myComA->setDataBits(DATA_7);
    if(ui->dataBitsComboBox->currentText()==tr("8"))
        myComA->setDataBits(DATA_8);
    //设置校验位
    myComA->setParity((ParityType)ui->parityComboBox->currentIndex());
    //设置停止位
    myComA->setStopBits((StopBitsType)ui->stopBitsComboBox->currentIndex());
    //设置数据流控制
    myComA->setFlowControl(FLOW_OFF);
    //设置延时
    myComA->setTimeout(TIME_OUT);
    setComboBoxEnabled(false);
    readTImerA->start(TIMER_INTERVAL);
    ui->StartUartbtn->setEnabled(false);
    ui->StopUartbtn->setEnabled(true);
    setupRealtimeDataDemo(ui->customPlot);

}
void MainWindow::readMycomA()
{
    bool ok;
    QByteArray temp=myComA->readAll();

    if(!temp.isEmpty())
    {
        if(temp.indexOf("CD")>=0)
        {
            acc_x=temp.mid((temp.indexOf("CD")+2),2).toHex().toInt(&ok,16);//将两个字节转化为16进制
            acc_y=temp.mid((temp.indexOf("CD")+4),2).toHex().toInt(&ok,16);
            plotFlag++;
            if(plotFlag>11)
                plotFlag=12;
        }
         //qDebug()<<"acc_x"<<acc_x;
    }

}
 //用于实时绘制温湿度实时数据曲线图的横坐标与纵坐标的数值范围
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
    //设置画布背景色
        /*   QLinearGradient plotGradient;
           plotGradient.setStart(0, 0);
           plotGradient.setFinalStop(0, 350);
           plotGradient.setColorAt(0, QColor(80, 80, 80));
           plotGradient.setColorAt(1, QColor(50, 50, 50));
           customPlot->setBackground(plotGradient);*/
           //设置坐标背景色
         /*        QLinearGradient axisRectGradient;
                 axisRectGradient.setStart(0, 0);
                 axisRectGradient.setFinalStop(0, 350);
                 axisRectGradient.setColorAt(0, QColor(80, 80, 80));
                 axisRectGradient.setColorAt(1, QColor(30, 30, 30));
                 customPlot->axisRect()->setBackground(axisRectGradient);*/

    //customPlot->setGeometry(10,20,1024,650);
    customPlot->addGraph();//添加温度曲线
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setName("Acc_X");
    //customPlot->graph(0)->setBrush(QBrush(QColor(240,250,200)));//设置曲线与横轴之间的颜色
    customPlot->graph(0)->setAntialiasedFill(false);

    customPlot->addGraph();//添加湿度曲线
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setName("Acc_Y");
    customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

    customPlot->addGraph();//添加温度曲线前端的扫描点
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);//数据点的不连接任何线
    //设置图中单数据点的视觉外观。如果设置为qcpscatterstyle：：ssnone，得出无散射点（如线只有情节适当的线式）
    //lsnoe代表实心圆点
    customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);

    customPlot->addGraph();//添加湿度曲线前端的扫描点
    customPlot->graph(3)->setPen(QPen(Qt::red));
    customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);//设置X轴的坐标为日期时间
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);//设置刻度步即2（主要）刻度的间隔，是否自动计算。如果设置为真，则该轴找到一个合理的步骤，这是合理的人类可读的图。
    customPlot->xAxis->setTickStep(2);//如果setautotickstep设置为false，使用这个函数来设置刻度步骤手动。在图坐标系中，刻度间隔是（主要）的刻度。
    customPlot->axisRect()->setupFullAxesBox();//方便的功能，创建一个轴的每一个方面，没有任何轴，并设置自己的能见度为真。此外，顶部/右轴被分配的底部/左轴的下面的属性：

    connect(customPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),customPlot->xAxis2,SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),customPlot->yAxis2,SLOT(setRange(QCPRange)));
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->legend->setVisible(true);//为图表添加标注
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
    //下面的函数为设置图表标注的位置
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect

    //允许用户拖动轴范围与鼠标，缩放鼠标滚轮，并通过点击选择图：
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    dataTimer->start(10);

}
void MainWindow::readtimeDataSlot() //用于将实时温湿度数据在曲线图上进行定时绘制
{
    double key=QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;//当前时间减去1970-01-01所得到的毫秒数

    static double lastPointKey=0;
    if(key-lastPointKey>0.01)//每10ms添加一次点
    {
        ui->customPlot->graph(0)->addData(key,acc_x);//温度曲线的值
        ui->customPlot->graph(1)->addData(key,acc_y);//湿度曲线的值

       // ui->customPlot->graph(2)->clearData();
       // ui->customPlot->graph(2)->addData(key,acc_x);//端点的值

       // ui->customPlot->graph(3)->clearData();
      //  ui->customPlot->graph(3)->addData(key,acc_y);

       // ui->customPlot->graph(0)->removeDataBefore(key-8);//只保留当前值与key-8之间的数据
        //ui->customPlot->graph(1)->removeDataBefore(key-8);
        if(plotFlag<10)
        {
            ui->customPlot->graph(0)->rescaleValueAxis(true);//重新调整坐标轴的范围
            ui->customPlot->graph(1)->rescaleValueAxis(true);
        }
        lastPointKey=key;

    }
    ui->customPlot->xAxis->setRange(key+0.2,10,Qt::AlignRight);
    ui->customPlot->replot();//重新绘制图像

}
