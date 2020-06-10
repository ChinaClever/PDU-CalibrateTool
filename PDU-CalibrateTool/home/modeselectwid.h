#ifndef MODESELECTWID_H
#define MODESELECTWID_H

#include "home_datatabwid.h"
#include "home_rangedlg.h"
#include "adjustcorethread.h"
#include "adjustzpduthread.h"

namespace Ui {
class ModeSelectWid;
}

class ModeSelectWid : public QWidget
{
    Q_OBJECT

public:
    explicit ModeSelectWid(QWidget *parent = nullptr);
    ~ModeSelectWid();

protected:
    void initSerial();
    bool initData();
    void setEnablWid(bool en);
    void endFun();
    void upTgWid();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();
    void on_modeBox_currentIndexChanged(int index);
    void on_onBtn_clicked();
    void on_downBtn_clicked();
    void on_errBtn_clicked();
    void on_deBtn_clicked();
    void on_reBtn_clicked();

private:
    Ui::ModeSelectWid *ui;
    sDataUnit *mData;
    sConfigItem *mItem;
    QTimer *timer;

    Home_RangeDlg *mRangeDlg;
    AdjustCoreThread *mCoreThread;
    AdjustMpduThread *mMpduThread;
    AdjustZpduThread *mZpduThread;


    SerialStatusWid *mSerialWid;
    SerialStatusWid *mSourceWid;
};

#endif // MODESELECTWID_H
