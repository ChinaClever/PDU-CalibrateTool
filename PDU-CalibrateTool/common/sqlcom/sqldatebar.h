#ifndef SQLDATEBAR_H
#define SQLDATEBAR_H

#include "sqlexportdlg.h"
#include "msgcalendardlg.h"

namespace Ui {
class SqlDateBar;
}

class SqlDateBar : public QWidget
{
    Q_OBJECT

public:
    explicit SqlDateBar(QWidget *parent = 0);
    ~SqlDateBar();

    QString getDate();

private slots:
    void on_startDateBtn_clicked();
    void on_endDateBtn_clicked();
    void on_dayRadio_clicked(bool checked);
    void on_weekRadio_clicked(bool checked);
    void on_monthRadio_clicked(bool checked);
    void on_currentMonthRadio_clicked(bool checked);

private:
    Ui::SqlDateBar *ui;
};

#endif // SQLDATEBAR_H
