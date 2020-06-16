/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "yc_standsource.h"

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

int YC_StandSource::write(QByteArray &array)
{
    int ret = 0;
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

int YC_StandSource::setRange()
{
    QString str = QString("I0 220 10 15 1200");
    QByteArray array = str.toLatin1();
    return write(array);
}

int YC_StandSource::powerDown()
{
    int ret = setRange();
    if(ret > 0) {
        QByteArray array = "V0";
        write(array);

        array = "A0";
        write(array);
    }

    return ret;
}

int YC_StandSource::powerOn(int v)
{
    int ret = setRange();
    if(ret > 0) {
        QByteArray array = "V100";
        write(array);

        array = "A" + QString::number(v).toUtf8();
        write(array);
    }

    return ret;
}

void YC_StandSource::powerReset()
{
    powerDown();
    msleep(500);
    powerOn();
}

void YC_StandSource::readScreenVal()
{
    QByteArray witeArray = "M";
    QByteArray readArray;
    read(witeArray,readArray,5);

    /////////////////////////////////////////////////////////debug
    qDebug()  << "[" << readArray.toHex() << "]"<<endl;
    /////////////////////////////////////////////////////////debug
}

