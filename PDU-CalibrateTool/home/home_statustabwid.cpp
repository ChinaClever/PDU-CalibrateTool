#include "home_statustabwid.h"

Home_StatusTabWid::Home_StatusTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mPacket = sDataPacket::bulid();
    mItem = Ad_Config::bulid()->item;
}

void Home_StatusTabWid::initWid()
{
    QString title = tr("校准数据列表");
    QStringList header;
    header<<tr("时间") << tr("项目");
    initTableWid(header, 0, title);
}


void Home_StatusTabWid::appendItem()
{
    if(mStatus != mPacket->status) {
        QStringList listStr;
        listStr  << QTime::currentTime().toString("hh:mm:ss");
        listStr << mPacket->status;

        insertRow(0, listStr);
        if(mPacket->pass == Test_Fail) {
            setAlarmBackgroundColor(0);
        } else {
            setNormalBackgroundColor(0);
        }

        mStatus = mPacket->status;
    }
}

void Home_StatusTabWid::startSlot()
{
    mStatus.clear();
    delTable();
}

void Home_StatusTabWid::timeoutDone()
{
    appendItem();
}
