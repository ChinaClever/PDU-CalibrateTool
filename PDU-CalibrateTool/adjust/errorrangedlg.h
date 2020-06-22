#ifndef ERRORRANGEDLG_H
#define ERRORRANGEDLG_H

#include <QDialog>
#include "adjustconfig.h"

namespace Ui {
class ErrorRangeDlg;
}

class ErrorRangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorRangeDlg(QWidget *parent = nullptr);
    ~ErrorRangeDlg();

    void initData(sConfigItem *item);
    void setModel(int index);

private slots:
    void on_okBtn_clicked();

private:
    Ui::ErrorRangeDlg *ui;
};

#endif // ERRORRANGEDLG_H
