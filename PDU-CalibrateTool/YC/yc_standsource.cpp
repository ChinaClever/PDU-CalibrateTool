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

bool YC_StandSource::write(QByteArray &array)
{
    bool ret = false;
    initSerialSlot();

    if(mSerial) {
        array.append(0x0D);
        int rtn = mSerial->write(array);
        if(rtn > 0) ret = true;
    }

    return ret;
}



bool YC_StandSource::setRange()
{
    QString str = QString("I0 220 10 15 1200");
    QByteArray array = str.toLatin1();

    return write(array);
}

bool YC_StandSource::setValue(const QString &str, int v)
{
    QByteArray array;
    array =  str.toLatin1() + QString::number(v).toUtf8();

    return write(array);
}


void YC_StandSource::powerDown()
{
    setValue("A", 0);
    setValue("V", 0);
}

bool YC_StandSource::powerOn(int v)
{
    bool ret = setValue("V", 100);
    if(ret) {
        // ret = setValue("A", v);
    }

    return ret;
}

bool YC_StandSource::setVol()
{
    setRange(); delay(1);
    bool ret = setValue("V", 100);
    if(ret) {
        ret = delay(6);
    }

    return ret;
}

bool YC_StandSource::setCur(int v)
{
    bool ret = setValue("A", v);
    if(ret) {
        ret = delay(10);
    }
    return ret;
}

bool YC_StandSource::powerReset()
{
    powerDown();
    bool ret = delay(6);
    if(ret) {
        ret = setVol();
        if(ret) {
            ret = setCur(60);
        }
    }

    return ret;
}

