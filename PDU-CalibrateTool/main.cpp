#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include "QtSingleApplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(QLatin1String("MPDU-CalibrateTool"),argc, argv);
    if(a.isRunning())  {
        QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("程序已运行！"),QMessageBox::Ok);
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
