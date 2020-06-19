#ifndef SETUPMAINWID_H
#define SETUPMAINWID_H

#include "ad_config.h"
#include "usermainwid.h"

namespace Ui {
class Setup_MainWid;
}

class Setup_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Setup_MainWid(QWidget *parent = 0);
    ~Setup_MainWid();

protected:
    void initSerial();
    void initErrData();
    void writeErrData();
    void updateErrData();
    void initLogCount();
    void initPcNum();

protected slots:
    void initFunSLot();

private slots:
    void on_logCountSpin_valueChanged(int arg1);
    void on_saveBtn_clicked();

    void on_pcNumSpin_valueChanged(int arg1);

private:
    Ui::Setup_MainWid *ui;
    UserMainWid *mUserWid;

    SerialStatusWid *mSerialWid;
    SerialStatusWid *mSourceWid;
};

#endif // SETUPMAINWID_H
