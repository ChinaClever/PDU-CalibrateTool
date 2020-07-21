#ifndef NEWUSRDLG_H
#define NEWUSRDLG_H

#include <QDialog>
#include "landinguser.h"
#include "msgbox.h"

namespace Ui {
class NewUsrDlg;
}

class NewUsrDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewUsrDlg(QWidget *parent = 0);
    ~NewUsrDlg();

    void editTitle(const QString &);
    void loadUsrInfo(sUserItem &user);

    virtual bool saveUsrInfo(sUserItem &user);
    void checkUsr(void);

protected:
    bool informationCheck(void);

private slots:
    void on_saveBtn_clicked();

private:
    Ui::NewUsrDlg *ui;
};

#endif // NEWUSRDLG_H
