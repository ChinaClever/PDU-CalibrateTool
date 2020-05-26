#ifndef MODESELECTWID_H
#define MODESELECTWID_H

#include "datatablewid.h"

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
    bool initData();
    void setEnablWid(bool en);
    void endFun();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();
    void on_modeBox_currentIndexChanged(int index);
    void on_onBtn_clicked();
    void on_downBtn_clicked();

private:
    Ui::ModeSelectWid *ui;
    sDataUnit *mData;
    sConfigItem *mItem;
    QTimer *timer;

    AdjustMpduThread *mMpduThread;
    AdjustZpduThread *mZpduThread;
};

#endif // MODESELECTWID_H
