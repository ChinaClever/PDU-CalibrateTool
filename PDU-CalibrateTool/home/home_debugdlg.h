#ifndef DEBUGDLG_H
#define DEBUGDLG_H

#include <QDialog>
#include "home_datatabwid.h"

namespace Ui {
class Home_DebugDlg;
}

class Home_DebugDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Home_DebugDlg(QWidget *parent = nullptr);
    ~Home_DebugDlg();

protected:
    void initThread();

private slots:
    void on_onAllBtn_clicked();
    void on_eleAllBtn_clicked();
    void on_closeAllBtn_clicked();
    void on_openBtn_clicked();
    void on_closeBtn_clicked();
    void on_eleBtn_clicked();
    void on_swAllBtn_clicked();
    void workDownSlot();

private:
    Ui::Home_DebugDlg *ui;
    int mId;
    sDataUnit *mData;
    sConfigItem *mItem;
    Ctrl_CoreThread *mThread;
};

#endif // DEBUGDLG_H
