#ifndef DEBUGDLG_H
#define DEBUGDLG_H

#include <QDialog>
#include "adjustzpduthread.h"

namespace Ui {
class DebugDlg;
}

class DebugDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DebugDlg(QWidget *parent = nullptr);
    ~DebugDlg();

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
    Ui::DebugDlg *ui;
    int mId;
    sDataUnit *mData;
    sConfigItem *mItem;
    AdjustCoreThread *mCoreThread;
};

#endif // DEBUGDLG_H
