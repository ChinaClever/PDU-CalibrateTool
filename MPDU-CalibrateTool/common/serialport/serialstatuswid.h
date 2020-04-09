#ifndef SERIALSTATUSWID_H
#define SERIALSTATUSWID_H

#include "serialportdlg.h"

namespace Ui {
class SerialStatusWid;
}

class SerialStatusWid : public QWidget
{
    Q_OBJECT

public:
    explicit SerialStatusWid(QWidget *parent = nullptr);
    ~SerialStatusWid();

    bool checkSerial();
    void disEnable() {mEnable=false;}
    SerialPort *getSerialPort(){return mSerialDlg->getSerialPort();}

protected:
    bool initSerialPort();
    void updateSerialWid();

private slots:
    void on_comBtn_clicked();

private:
    Ui::SerialStatusWid *ui;

    bool mEnable;
   // sTestConfigItem *mItem;
    SerialPortDlg *mSerialDlg;
};

#endif // SERIALSTATUSWID_H
