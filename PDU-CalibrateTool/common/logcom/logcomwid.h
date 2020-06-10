#ifndef LOGCOMWID_H
#define LOGCOMWID_H

#include "logbtnbar.h"
#include "log_datewidget.h"

namespace Ui {
class LogComWid;
}

class LogComWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogComWid(QWidget *parent = 0);
    ~LogComWid();

    void initWid(BasicSql *db, LogBtnBar *bar,  SqlQueryBase *btn);

protected:
    Ui::LogComWid *ui;
    SqlTableWid *mSqlTableWid;
    LOG_DateWidget *mDateWidget;
};

#endif // LOGCOMWID_H
