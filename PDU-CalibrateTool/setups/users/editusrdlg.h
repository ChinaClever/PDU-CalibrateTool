#ifndef EDITUSRDLG_H
#define EDITUSRDLG_H
#include "newusrdlg.h"

class EditUsrDlg : public NewUsrDlg
{
    Q_OBJECT

public:
    EditUsrDlg(QWidget *parent = 0);
    ~EditUsrDlg();

    void setUsrId(int id);

protected:
     bool saveUsrInfo(sUserItem &user);
private:
     int mId;
};

#endif // EDITUSRDLG_H
