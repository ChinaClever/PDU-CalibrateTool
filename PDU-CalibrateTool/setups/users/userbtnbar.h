#ifndef USERBTNBAR_H
#define USERBTNBAR_H
#include "usrlanddlg.h"
#include "sqltablewid.h"

class UserBtnBar : public SqlBtnBar
{
    Q_OBJECT
public:
    explicit UserBtnBar(QWidget *parent = nullptr);

signals:

protected :
     void addBtn();
     bool delBtn(int id);
     void modifyBtn(int id);
};

#endif // USERBTNBAR_H
