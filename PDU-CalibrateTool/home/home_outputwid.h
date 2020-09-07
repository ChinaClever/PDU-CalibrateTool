#ifndef HOME_OUTPUTWID_H
#define HOME_OUTPUTWID_H

#include "home_thresholdwid.h"

namespace Ui {
class Home_OutputWid;
}

class Home_OutputWid : public QWidget
{
    Q_OBJECT

public:
    explicit Home_OutputWid(QWidget *parent = nullptr);
    ~Home_OutputWid();

protected:
    void initThread();

private slots:
    void on_onAllBtn_clicked();
    void on_eleAllBtn_clicked();
    void on_closeAllBtn_clicked();
    void on_openBtn_clicked();
    void on_closeBtn_clicked();
    void on_eleBtn_clicked();
    void on_swAllBtn_clicked();
    void on_onBtn_clicked();
    void on_downBtn_clicked();
    void workDownSlot();
    void initFunSlot();

private:
    Ui::Home_OutputWid *ui;
    int mId;
    sDataUnit *mData;
    Ctrl_CoreThread *mThread;
};

#endif // HOME_OUTPUTWID_H
