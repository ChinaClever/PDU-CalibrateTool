/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_thresholdwid.h"

Home_ThresholdWid::Home_ThresholdWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    groupBox_background_icon(this);
    mData = sDataPacket::bulid()->data;
}

void Home_ThresholdWid::initWid()
{
    QString title = tr("设备阈值状态");
    QStringList header;
    header << tr("项目") << tr("当前值") << tr("最小值") << tr("最大值");
    initTableWid(header, 0, title);
}

void Home_ThresholdWid::appendItem(sDataUnit *unit)
{
    int k=0;
    for(int i=0; i<unit->size; ++i) {
        QStringList listStr;
        listStr << tr("L%1相电压").arg(i+1);
        listStr << QString::number(unit->vol[i]/(unit->rate),'f',1)+"V";
        listStr << QString::number(unit->volTh.min[i]/(unit->rate),'f',1)+"V";
        listStr << QString::number(unit->volTh.max[i]/(unit->rate),'f',1)+"V";
        setTableRow(k, listStr);
        if((unit->vol[i] < unit->volTh.min[i]) || (unit->vol[i] > unit->volTh.max[i])){
            setAlarmBackgroundColor(k);
        } else {
            setNormalBackgroundColor(k);
        }
        k += 1;
    }

    for(int i=0; i<unit->size; ++i) {
        QStringList listStr;
        listStr << tr("L%1相电流").arg(i+1);
        listStr << QString::number(unit->cur[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        listStr << QString::number(unit->curTh.min[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        listStr << QString::number(unit->curTh.max[i]/(unit->rate * COM_RATE_CUR),'f',2)+"A";
        setTableRow(k, listStr);
        if((unit->cur[i] < unit->curTh.min[i]) || (unit->cur[i] > unit->curTh.max[i])){
            setAlarmBackgroundColor(k);
        } else {
            setNormalBackgroundColor(k);
        }
        k += 1;
    }
}

void Home_ThresholdWid::timeoutDone()
{
    uchar devType = sDataPacket::bulid()->devType->devType;
    if(devType < RPDU_Mc) return;
    if(mData->size){
        appendItem(mData);
    } else {
        clearTable();
    }
}
