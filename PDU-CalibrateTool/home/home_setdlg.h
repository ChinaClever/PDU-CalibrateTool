#ifndef HOME_SETDLG_H
#define HOME_SETDLG_H

#include <QDialog>
#include "home_datatabwid.h"

namespace Ui {
class Home_SetDlg;
}

class Home_SetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Home_SetDlg(QWidget *parent = nullptr);
    ~Home_SetDlg();

    void initThresholdWid();
protected:
    void getThresholdWid();
    void setThresholdWid();

private slots:
    void on_okBtn_clicked();
    void on_checkBox_clicked(bool checked);
    void on_comboBox_currentIndexChanged(int index);
    void on_ipTypeBox_currentIndexChanged(int index);

private:
    Ui::Home_SetDlg *ui;
    sConfigItem *mItem;
};

#endif // HOME_SETDLG_H
