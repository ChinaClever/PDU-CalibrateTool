#ifndef USRLANDDLG_H
#define USRLANDDLG_H

#include <QDialog>
#include "editusrdlg.h"

namespace Ui {
class UsrLandDlg;
}

class UsrLandDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UsrLandDlg(QWidget *parent = 0);
    ~UsrLandDlg();

    bool quitWidget(void);
    int selectWork(void);
    void usrLand();
signals:
    void sendUserNameSig(QString str);

public slots:
    void on_landBtn_clicked();

private slots:
    void closeEvent(QCloseEvent *event);

private:
    Ui::UsrLandDlg *ui;
};

#endif // USRLANDDLG_H
