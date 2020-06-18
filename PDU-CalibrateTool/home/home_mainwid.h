#ifndef HOMEMAINWID_H
#define HOMEMAINWID_H

#include <QWidget>
#include "serialstatuswid.h"
#include "home_workwid.h"

namespace Ui {
class Home_MainWid;
}

class Home_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_MainWid(QWidget *parent = nullptr);
    ~Home_MainWid();

protected slots:
    void initFunSlot();

private:
    Ui::Home_MainWid *ui;
    Home_DataTabWid *mTableWid;
    Home_WorkWid *mModeWid;
};

#endif // HOMEMAINWID_H
