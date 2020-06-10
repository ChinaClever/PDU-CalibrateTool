#ifndef SQL_EXPORTDLG_H
#define SQL_EXPORTDLG_H

#include <QDialog>
#include <QFileDialog>
#include "msgbox.h"
#include "excel_savethread.h"

namespace Ui {
class SqlExportDlg;
}

class SqlExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SqlExportDlg(QWidget *parent = 0);
    ~SqlExportDlg();

    void init(const QString &, QList<QStringList> &list);

protected:
    bool checkInput();
    void exportDone();

protected slots:
    void timeoutDone();
    void on_pushButton_clicked();
    void on_exportBtn_clicked();
    void on_quitBtn_clicked();

private:
    Ui::SqlExportDlg *ui;
    QTimer *timer;
    QList<QStringList> mList;
    Excel_SaveThread *mExportThread;
};

#endif // LOG_EXPORTDLG_H
