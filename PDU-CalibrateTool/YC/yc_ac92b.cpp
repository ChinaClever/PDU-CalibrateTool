/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "yc_ac92b.h"

YC_Ac92b::YC_Ac92b(QObject *parent) : YC_StandSource(parent)
{

}

YC_Ac92b *YC_Ac92b::bulid(QObject *parent)
{
    static YC_Ac92b* sington = nullptr;
    if(sington == nullptr)
        sington = new YC_Ac92b(parent);
    return sington;
}

bool YC_Ac92b::serialWrite(QByteArray &array)
{
    bool ret = false;
    int rtn = mSerial->write(array);
    if(rtn > 0) ret = true;

    return ret;
}


bool YC_Ac92b::setValue(const QString &str, int v)
{
    QByteArray array;
    array =  str.toLatin1() + QString::number(v).toUtf8();

    return write(array);
}

bool YC_Ac92b::setRange()
{
    QString str = QString("I0 220 10 15 1200");
    QByteArray array = str.toLatin1();

    return write(array);
}

void YC_Ac92b::powerDown()
{
    setValue("A", 0);
    setValue("V", 0);
}


bool YC_Ac92b::powerOn(int v)
{
    bool ret = setValue("V", 100);
    if(ret) {
        // ret = setValue("A", v);
    }

    return ret;
}


bool YC_Ac92b::setVol()
{
    setRange(); delay(1);
    bool ret = setValue("V", 100);
    if(ret) {
        ret = delay(6);
    }

    return ret;
}

bool YC_Ac92b::setCur(int v)
{
    bool ret = setValue("A", v);
    if(ret) {
        ret = delay(10);
    }
    return ret;
}


bool YC_Ac92b::handShake()
{
    QByteArray array;
    array.append("H0\r");

    bool ret = setBaudRate(9600);
    if(ret) {
        QByteArray res;
        int rtn = mSerial->transmit(array, res, 2);
        if(rtn > 0)  {
            acOrDc = 1;
            ret = true;
        } else {
            acOrDc = 0;
            ret = false;
        }
    }

    return ret;
}