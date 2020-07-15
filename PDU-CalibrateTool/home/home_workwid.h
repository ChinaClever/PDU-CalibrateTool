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
    void upStatusLab();
    void upLabColor();
    QString getTime();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();
    void on_deBtn_clicked();
    void on_resBtn_clicked();
    void on_updateBtn_clicked();

private:
    Ui::Home_WorkWid *ui;
    Ad_CoreThread *mCore;
    sConfigItem *mItem;
    QTimer *timer;
};

#endif // MODESELECTWID_H
