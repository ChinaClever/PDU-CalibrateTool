#ifndef HOME_SETDLG_H
#define HOME_SETDLG_H

#include <QDialog>
#include "home_statustabwid.h"

namespace Ui {
class Home_SetDlg;
}

class Home_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Home_SetDlg(QWidget *parent = nullptr);
    ~Home_SetDlg();

protected:
    void updateSiLed();

public slots:
    void initFunSlot();

private slots:
    void on_okBtn_clicked();
    void on_resBtn_clicked();

private:
    Ui::Home_SetDlg *ui;
    sConfigItem *mItem;
};

#endif // HOME_SETDLG_H
