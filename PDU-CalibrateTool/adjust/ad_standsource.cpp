#include "ad_standsource.h"

Ad_StandSource::Ad_StandSource(QObject *parent) : QThread(parent)
{
    mSerial = AdjustConfig::bulid()->item->source;
}


Ad_StandSource *Ad_StandSource::bulid(QObject *parent)
{
    static Ad_StandSource* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_StandSource(parent);
    return sington;
}

int Ad_StandSource::write(QByteArray &array)
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

int Ad_StandSource::read(QByteArray &witeArray, QByteArray &readArray, int msecs)
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

int Ad_StandSource::setRange()
{
    sConfigItem *item = AdjustConfig::bulid()->item;
    QString str = QString("I0 %1 %2 15 1200").arg(item->vol).arg(item->cur);
    QByteArray array = str.toLatin1();
    return write(array);
}

int Ad_StandSource::powerDown()
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

int Ad_StandSource::powerOn()
{
    int ret = setRange();
    if(ret > 0) {
        QByteArray array = "V100";
        write(array);

        array = "A100";
        write(array);
    }

    return ret;
}

void Ad_StandSource::powerReset()
{
    powerDown();
    msleep(500);
    powerOn();
}

void Ad_StandSource::readScreenVal()
{
    QByteArray witeArray = "M";
    QByteArray readArray;
    read(witeArray,readArray,5);

    /////////////////////////////////////////////////////////debug
    qDebug()  << "[" << readArray.toHex() << "]"<<endl;
    /////////////////////////////////////////////////////////debug
}

