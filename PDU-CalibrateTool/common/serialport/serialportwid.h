#ifndef SERIALPORTWID_H
#define SERIALPORTWID_H

#include <QWidget>
#include "serialport.h"

namespace Ui {
class SerialPortWid;
}

class SerialPortWid : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortWid(QWidget *parent = 0);
    ~SerialPortWid();

    SerialPort *getSerialPort(){return mSerialPort;}
    void updateBaudRate(qint32 br);

protected:
    void initWidget();
    void initSerialPort();

signals:
    void closeSignal();

protected slots:
    void statusSlot();
    void workBtnSlot();
    void updateBtnSlot();
    void openSerialSlot();

//private slots:
//    void on_loopBtn_clicked();

private:
    Ui::SerialPortWid *ui;
    SerialPort *mSerialPort;
};
extern void groupBox_background_icon(QWidget *target);

#endif // SERIALPORTWID_H
