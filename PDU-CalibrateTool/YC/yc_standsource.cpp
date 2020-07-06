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


YC_StandSource *YC_StandSource::bulid(QObject *parent)
{
    static YC_StandSource* sington = nullptr;
    if(sington == nullptr)
        sington = new YC_StandSource(parent);
    return sington;
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

int YC_StandSource::write(QByteArray &array)
{
    int ret = 0;
    initSerialSlot();

    if(mSerial) {
        array.append(0x0D);
        if(mSerial->isOpened()) {
            ret = mSerial->send(array);
            msleep(450);
        }
    }

    return ret;
}

int YC_StandSource::read(QByteArray &witeArray, QByteArray &readArray, int msecs)
{
    int ret = 0;
    if(mSerial) {
        witeArray.append(0x0D);
        if(mSerial->isOpened()) {
            ret = mSerial->transmit(witeArray,readArray,msecs);
            msleep(450);
        }
    }

    return ret;
}

bool YC_StandSource::setRange()
{
    bool ret = false;

    QString str = QString("I0 220 10 15 1200");
    QByteArray array = str.toLatin1();
    int rtn = write(array);
    if(rtn > 0) {
        ret = true;
    }

    return ret;
}

void YC_StandSource::powerDown()
{
    QByteArray array = "V0";
    write(array);

    array = "A0";
    write(array);
}

bool YC_StandSource::powerOn(int v)
{
    bool ret = setRange();
    if(ret) {
        QByteArray array = "V100";
        write(array);

        array = "A" + QString::number(v).toUtf8();
        write(array);
    }

    return ret;
}

bool YC_StandSource::powerReset()
{
    powerDown();
    delay(5);
    return powerOn();
}

