#ifndef LOG_PDUALARMQUERYDLG_H
#define LOG_PDUALARMQUERYDLG_H

#include <QDialog>
//#include "log_macbtnbar.h"
#include "logcomwid.h"
#include "dblogs.h"

namespace Ui {
class Log_LogQueryDlg;
}

class Log_LogQueryDlg : public QDialog, public SqlQueryBase
{
    Q_OBJECT

public:
    explicit Log_LogQueryDlg(QWidget *parent = 0);
    ~Log_LogQueryDlg();

    QString getCmd();
    int Exec() {return this->exec();}
    virtual bool inputCheck();

protected slots:
    void on_quitBtn_clicked();
    void on_okBtn_clicked();

protected:
    Ui::Log_LogQueryDlg *ui;
    SqlDateBar *mDateBar;
};

#endif // LOG_PDUALARMQUERYDLG_H
