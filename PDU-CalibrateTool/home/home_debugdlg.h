#ifndef DEBUGDLG_H
#define DEBUGDLG_H

#include <QDialog>
#include "home_outputwid.h"

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
    void initWid();

private:
    Ui::Home_DebugDlg *ui;
    Home_OutputWid *mOutputWid;
    Home_ThresholdWid *mThresholdWid;
};

#endif // DEBUGDLG_H
