#ifndef ERRORRANGEDLG_H
#define ERRORRANGEDLG_H

#include <QDialog>
#include "adjustconfig.h"

namespace Ui {
class Home_RangeDlg;
}

class Home_RangeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Home_RangeDlg(QWidget *parent = nullptr);
    ~Home_RangeDlg();

    void initData(sConfigItem *item);
    void setModel(int index);

private slots:
    void on_okBtn_clicked();

private:
    Ui::Home_RangeDlg *ui;
};

#endif // ERRORRANGEDLG_H
