#ifndef MSGCALENDARDLG_H
#define MSGCALENDARDLG_H

#include <QDialog>
#include <QDate>
#include "msgbox.h"

namespace Ui {
class MsgCalendarDlg;
}

class MsgCalendarDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MsgCalendarDlg(QWidget *parent = 0);
    ~MsgCalendarDlg();

    QDate getDate() {return mDate;}

protected slots:
    void selectionChangedSLot();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MsgCalendarDlg *ui;
    QDate mDate;
};

#endif // LOGCALENDARDLG_H
