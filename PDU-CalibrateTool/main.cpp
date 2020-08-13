#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include "QtSingleApplication.h"

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3").arg(text).arg(msg).arg(current_date);//..arg(context_info);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);

    if(msg.length() < 3) text_stream << "\r\n";
    else text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(outputMessage);//注释显示打印信息，打开则输出日志
    QtSingleApplication a(QLatin1String("PDU-CalibrateTool"),argc, argv);
    if(a.isRunning())  {
        QMessageBox::information(NULL, QObject::tr("提示"), QObject::tr("程序已运行！"),QMessageBox::Ok);
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
