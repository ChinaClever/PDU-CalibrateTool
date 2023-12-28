#include "home_statustabwid.h"

Home_StatusTabWid::Home_StatusTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mPacket = sDataPacket::bulid();
    mItem = Ad_Config::bulid()->item;
    groupBox_background_icon(this);
}

void Home_StatusTabWid::initWid()
{
    QStringList header;
    QString title = tr("校准数据列表");
    header<<tr("时间") << tr("项目");
    initTableWid(header, 0, title);

    setColumnWidth(0, 200);
}

void Home_StatusTabWid::appendItem()
{
    if(mStatus != mPacket->status) {
        QStringList listStr;
        listStr << QTime::currentTime().toString("hh:mm:ss");
        listStr << mPacket->status;

        insertRow(0, listStr);
        setAlignLeft(0, 1);
        if(mPacket->pass == Test_Fail) {
            setAlarmBackgroundColor(0);
        } else {
            setNormalBackgroundColor(0);
        }

        mStatus = mPacket->status;
        mPacket->getPro()->uploadPass << mPacket->getPro()->uploadPassResult;
        mPacket->getPro()->itemName << mStatus;
    }
}

void Home_StatusTabWid::startSlot()
{
    mStatus.clear();
    delTable();
}

void Home_StatusTabWid::timeoutDone()
{
    if(!mPacket->status.isEmpty()) {
        appendItem();
    }
}
