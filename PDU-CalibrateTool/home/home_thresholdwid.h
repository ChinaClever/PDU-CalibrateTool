#ifndef HOME_THRESHOLDWID_H
#define HOME_THRESHOLDWID_H

#include "home_datatabwid.h"

class Home_ThresholdWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Home_ThresholdWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void appendItem(sDataUnit *unit);

protected slots:
    void timeoutDone();

private:
    sDataUnit *mData;
};

#endif // HOME_THRESHOLDWID_H
