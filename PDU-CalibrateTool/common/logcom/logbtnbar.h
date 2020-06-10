#ifndef LOGBTNBAR_H
#define LOGBTNBAR_H

#include "sqlquerybase.h"

class LogBtnBar : public SqlBtnBar
{
    Q_OBJECT
public:
    explicit LogBtnBar(QWidget *parent = nullptr);
    void setDlg(SqlQueryBase *dlg){mQueryDlg=dlg;}

protected:
    virtual QString queryBtn();

protected:
    SqlQueryBase *mQueryDlg;
};

#endif // LOGBTNBAR_H
