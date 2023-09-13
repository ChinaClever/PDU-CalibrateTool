#ifndef LOGMAINWID_H
#define LOGMAINWID_H
#include "dbvalue.h"
#include "logquerydlg/log_logquerydlg.h"

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
    LogComWid *mValueWid;
};

#endif // LOGMAINWID_H
