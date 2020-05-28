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

private:
    Ui::DebugDlg *ui;
    AdjustCoreThread *mCoreThread;
};

#endif // DEBUGDLG_H
