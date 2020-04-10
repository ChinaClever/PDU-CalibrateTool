#ifndef MODESELECTWID_H
#define MODESELECTWID_H

#include "datatablewid.h"

namespace Ui {
class ModeSelectWid;
}

class ModeSelectWid : public QWidget
{
    Q_OBJECT

public:
    explicit ModeSelectWid(QWidget *parent = nullptr);
    ~ModeSelectWid();

protected:
    void initData();

private slots:
    void timeoutDone();
    void on_startBtn_clicked();
    void on_modeBox_currentIndexChanged(int index);

private:
    Ui::ModeSelectWid *ui;
    sDataUnit *mData;
    sConfigItem *mItem;
    QTimer *timer;
};

#endif // MODESELECTWID_H
