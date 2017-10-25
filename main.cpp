#include "mainwindow.h"
#include <QApplication>
#include<QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));//设置系统中文显示
    MainWindow w;
    w.show();

    return a.exec();
}
