#ifndef HOME_STATUSTABWID_H
#define HOME_STATUSTABWID_H

#include "home_datatabwid.h"

class Home_StatusTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Home_StatusTabWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void appendItem();

public slots:
    void startSlot();

protected slots:
    void timeoutDone();

private:
    QString mStatus;
    sDataPacket *mPacket;
    sConfigItem *mItem;
};

#endif // HOME_STATUSTABWID_H
