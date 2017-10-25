#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include"win_qextserialport.h"
#include <QMessageBox>
#include<QFile>
#include<QTimer>
#include"qcustomplot.h"
#define TIME_OUT 10      //延时，TIME_OUT是串口读写的延时
#define TIMER_INTERVAL 50//读取定时器时间间隔，200ms


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_StartUartbtn_clicked();
    void on_StopUartbtn_clicked();
    void readMycomA();                  //读取串口
    void readtimeDataSlot();



private:
    void startInit();                   //初始化
    //用于实时绘制温湿度实时数据曲线图的横坐标与纵坐标的数值范围
    void setupRealtimeDataDemo(QCustomPlot*customPlot);
    int timerdly;
    void setComboBoxEnabled(bool status);
    Win_QextSerialPort *myComA;            //定义读GPS端口
    QTimer *readTImerA;                      //定义用于读取温湿度的定时器
    QTimer *dataTimer;                       //定义用于绘制曲线的定时器
typedef union
{
        short value;
        char byte[2];
}d_byte;
    short acc_x=0;                           //温度数据
    short acc_y=0;                            //湿度数据
    int plotFlag=0;
};

#endif // MAINWINDOW_H
