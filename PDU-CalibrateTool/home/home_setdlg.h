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
    void on_aiBox_currentIndexChanged(int index);

    void on_userEdit_selectionChanged();

private:
    Ui::Home_SetDlg *ui;
    sConfigItem *mItem;
    sDataPacket *packet;
};

#endif // HOME_SETDLG_H
