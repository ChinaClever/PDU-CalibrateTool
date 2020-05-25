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
            ret = mSerial->write(array);
             msleep(350);
        }
    }

    return ret;
}

void StandardSource::setRange()
{
    QByteArray array = "I0 220 6 15 1200";
    write(array);
}

void StandardSource::powerDown()
{
    setRange();
    QByteArray array = "V0";
    write(array);

    array = "A0";
    write(array);
}

void StandardSource::powerOn()
{
    setRange();
    QByteArray array = "V100";
    write(array);

    array = "A100";
    write(array);
}

void StandardSource::powerReset()
{
    powerDown();
    powerOn();
}
