#include "standardsource.h"

StandardSource::StandardSource(QObject *parent) : QThread(parent)
{
    mSerial = AdjustConfig::bulid()->item->source;
}


StandardSource *StandardSource::bulid(QObject *parent)
{
    static StandardSource* sington = nullptr;
    if(sington == nullptr)
        sington = new StandardSource(parent);
    return sington;
}

int StandardSource::write(QByteArray &array)
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

int StandardSource::read(QByteArray &witeArray, QByteArray &readArray, int msecs)
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

int StandardSource::setRange()
{
    QByteArray array = "I0 220 6 15 1200";
    return write(array);
}

int StandardSource::powerDown()
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

int StandardSource::powerOn()
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

void StandardSource::powerReset()
{
    powerDown();
    msleep(500);
    powerOn();
}

void StandardSource::readScreenVal()
{
    QByteArray witeArray = "M";
    QByteArray readArray;
    read(witeArray,readArray,5);

    /////////////////////////////////////////////////////////debug
    qDebug()  << "[" << readArray.toHex() << "]"<<endl;
    /////////////////////////////////////////////////////////debug
}

