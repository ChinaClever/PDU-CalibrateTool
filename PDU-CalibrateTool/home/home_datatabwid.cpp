/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_datatabwid.h"

Home_DataTabWid::Home_DataTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mData = sDataPacket::bulid()->data;
    mItem = Ad_Config::bulid()->item;
}

void Home_DataTabWid::initWid()
{
    QString title = tr("状态列表");
    QStringList header;
    header<<tr("开关") << tr("电流") << tr("电压") << tr("功率") << tr("电能") << tr("最终电流")<< tr("结果");
    initTableWid(header, 0, title);
}


void Home_DataTabWid::appendItem(sDataUnit *unit)
{
    for(int i=0; i<unit->size; ++i) {
        QStringList listStr;

        if(1 == unit->sw[i]) listStr << tr("开"); else listStr << tr("关");
        listStr << QString::number(unit->cur[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        listStr << QString::number(unit->vol[i]/(unit->rate),'f',1)+"V";
        listStr << QString::number(unit->pow[i]/(unit->rate * COM_RATE_POW),'f',4)+"Kw";
        listStr << QString::number(unit->ele[i]/(unit->rate * COM_RATE_ELE),'f',2)+"Kwh";
        if(unit->cured[i]) {
            listStr << QString::number(unit->cured[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        } else {
            listStr << "---";
        }

        switch (unit->status[i]) {
        case Test_Fail: listStr << "×"; break;
        case Test_Success: listStr << "√"; break;
        default: listStr << " "; break;
        }

        setTableRow(i, listStr);
        if(unit->status[i] == Test_Fail) {
            setAlarmBackgroundColor(i);
        } else {
            setNormalBackgroundColor(i);
        }
    }
}

void Home_DataTabWid::timeoutDone()
{
    if(mData->size) {
        appendItem(mData);
    } else {
        clearTable();
    }
}
