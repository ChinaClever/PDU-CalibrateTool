#ifndef MODESELECTWID_H
#define MODESELECTWID_H

#include "ad_corethread.h"
#include "home_setdlg.h"

namespace Ui {
class Home_WorkWid;
}

class Home_WorkWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_WorkWid(QWidget *parent = nullptr);
    ~Home_WorkWid();

signals:
    void startSig();

protected:
    bool initWid();
    void endFun();
    void upTgWid();
    void upStatusLab();
    void upLabColor();
    QString getTime();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();
    void on_deBtn_clicked();
    void on_resBtn_clicked();
    void on_updateBtn_clicked();
    void on_setBtn_clicked();

    void on_addrSpin_valueChanged(int arg1);

private:
    Ui::Home_WorkWid *ui;
    Home_SetDlg *mSetDlg;
    Ad_CoreThread *mCore;
    sConfigItem *mItem;
    QTimer *timer;
};

#endif // MODESELECTWID_H
