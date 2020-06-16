#ifndef MODESELECTWID_H
#define MODESELECTWID_H

#include "home_datatabwid.h"
#include "ad_corethread.h"


namespace Ui {
class Home_WorkWid;
}

class Home_WorkWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_WorkWid(QWidget *parent = nullptr);
    ~Home_WorkWid();

protected:
    bool initWid();
    void endFun();
    void upTgWid();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();

    void on_onBtn_clicked();
    void on_downBtn_clicked();
    void on_deBtn_clicked();
    void on_reBtn_clicked();

private:
    Ui::Home_WorkWid *ui;
    sDataUnit *mData;
    sConfigItem *mItem;
    QTimer *timer;

    Ad_CoreThread *mCore;
};

#endif // MODESELECTWID_H
