#ifndef NAVBARWID_H
#define NAVBARWID_H

#include <QWidget>
#include "usrlanddlg.h"

namespace Ui {
class NavBarWid;
}

class NavBarWid : public QWidget
{
    Q_OBJECT

public:
    explicit NavBarWid(QWidget *parent = nullptr);
    ~NavBarWid();

signals:
    void navBarSig(int);

private slots:
    void on_homeBtn_clicked();
    void on_setBtn_clicked();
    void on_logBtn_clicked();
    void on_loginBtn_clicked();
    void on_readMeBtn_clicked();

private:
    Ui::NavBarWid *ui;
    UsrLandDlg* mUserLand;
};

#endif // NAVBARWID_H
