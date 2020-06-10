#ifndef SETUPMAINWID_H
#define SETUPMAINWID_H

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
    void initPdusTable();


protected slots:
    void initFunSLot();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_logCountSpin_valueChanged(int arg1);

private:
    Ui::Setup_MainWid *ui;
    UserMainWid *mUserWid;
};

#endif // SETUPMAINWID_H
