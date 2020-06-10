#ifndef USERMAINWID_H
#define USERMAINWID_H

#include "userbtnbar.h"


namespace Ui {
class UserMainWid;
}

class UserMainWid : public QWidget
{
    Q_OBJECT

public:
    explicit UserMainWid(QWidget *parent = 0);
    ~UserMainWid();

private:
    Ui::UserMainWid *ui;
    UserBtnBar *mBtnBar;
    SqlTableWid *mDbTable;
};

#endif // USERMAINWID_H
