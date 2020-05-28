/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "datatablewid.h"

DataTableWid::DataTableWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;
}

void DataTableWid::initWid()
{
    QString title = tr("输出位状态列表");
    QStringList header;
    header<<tr("开关") << tr("电流") << tr("电压") << tr("功率") << tr("电能") << tr("结果");
    initTableWid(header, 0, title);
}


void DataTableWid::appendItem(sDataUnit *unit)
{
    for(int i=0; i<unit->size; ++i) {
        QStringList listStr;

        if(unit->sw) listStr << tr("开"); else listStr << tr("关");
        listStr << QString::number(unit->cur[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        listStr << QString::number(unit->vol[i]/(unit->rate),'f',1)+"V";
        listStr << QString::number(unit->pow[i]/(unit->rate * COM_RATE_POW),'f',4)+"Kw";
        listStr << QString::number(unit->ele[i]/(unit->rate * COM_RATE_ELE),'f',2)+"Kwh";

        switch (unit->status[i]) {
        case 0: listStr << " "; break;
        case 1: listStr << "√"; break;
        case 2: listStr << "×"; break;
        }

        setTableRow(i, listStr);
        if(unit->status[i] == 2) {
            setBackgroundColor(i);
        }
    }
}

void DataTableWid::timeoutDone()
{
    if(mData->size) {
        appendItem(mData);
    } else {
        clearTable();
    }
}
