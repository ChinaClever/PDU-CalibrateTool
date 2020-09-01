#ifndef LOG_PDUMACQUERYDLG_H
#define LOG_PDUMACQUERYDLG_H

#include "log_logquerydlg.h"

namespace Ui {
class Log_MacQueryDlg;
}

class Log_MacQueryDlg : public QDialog, public SqlQueryBase
{
    Q_OBJECT

public:
    explicit Log_MacQueryDlg(QWidget *parent = 0);
    ~Log_MacQueryDlg();

    QString getCmd();
    int Exec() {return this->exec();}
    virtual bool inputCheck();

protected slots:
    void on_quitBtn_clicked();
    void on_okBtn_clicked();

protected:
    Ui::Log_MacQueryDlg *ui;
    SqlDateBar *mDateBar;
};

#endif // LOG_PDUALARMQUERYDLG_H
