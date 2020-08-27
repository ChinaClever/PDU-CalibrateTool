#ifndef LOGMAINWID_H
#define LOGMAINWID_H
#include "dbstates.h"
#include "logquerydlg/log_macquerydlg.h"

namespace Ui {
class LogMainWid;
}

class LogMainWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogMainWid(QWidget *parent = 0);
    ~LogMainWid();

protected slots:
    void initFunSLot();

private slots:

private:
    Ui::LogMainWid *ui;
    LogComWid *mLogWid;
    LogComWid *mMacWid;
    LogComWid *mStatusWid;
};

#endif // LOGMAINWID_H
