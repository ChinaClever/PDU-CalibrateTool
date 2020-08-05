/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "yc_standsource.h"
#include "ad_modbus.h"

YC_StandSource::YC_StandSource(QObject *parent) : QThread(parent)
{
    mSerial = nullptr;
    QTimer::singleShot(1000,this,SLOT(initSerialSlot()));
}

void YC_StandSource::initSerialSlot()
{
    if(!mSerial) {
        mSerial = Ad_Config::bulid()->item->source;
    }
}

bool YC_StandSource::delay(int s)
{
    return Ad_Modbus::bulid(this)->delay(s);
}

bool YC_StandSource::setBaudRate(qint32 baudRate)
{
    initSerialSlot();
    return mSerial->setBaudRate(baudRate);
}

bool YC_StandSource::write(QByteArray &array)
{
    bool ret = false;
    initSerialSlot();

    if(mSerial) {
        array.append(0x0D);
        ret = serialWrite(array);
    }

    return ret;
}


bool YC_StandSource::powerReset()
{
    powerDown();
    bool ret = delay(6);
    if(ret) {
        ret = setVol(220);
        if(ret) {
            ret = setCur(60);
        }
    }

    return ret;
}

