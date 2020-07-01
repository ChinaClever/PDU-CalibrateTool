/*
 *
 *  Created on: 2020年10月1日
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

int YC_StandSource::readScreenVal(float targetCur,int delay)
{
    QByteArray witeArray = "M";
    QByteArray readArray;
    int ret = read(witeArray,readArray,delay);
    if(ret == 0)
        return -1;//没有读取到数据

    /////////////////////////////////////////////////////////debug
    qDebug()  << "[" << readArray.toHex() << "]"<<endl;

    qDebug()<<"readArray len"<<readArray.length()<<endl;
    for(int i = 0 ; i < readArray.length();i+=4)
    {
        unsigned char s[4];
        //32位
        s[0]=readArray.at(i);
        s[1]=readArray.at(i+1);
        s[2]=readArray.at(i+2);
        s[3]=readArray.at(i+3);
        float f2;
        memcpy_s(&f2 , sizeof(float) , s , 4);
        qDebug()<<f2<<endl;
        if(i == 2)
        {
            if(fabs(targetCur-f2)<1e-6)
                return 1;//刚好相等
        }
    }
    //    unsigned char s[4];
    //     //32位
    //     s[0]=0xCC;
    //     s[1]=0x03;
    //     s[2]=0x5C;
    //     s[3]=0x43;
    //     float f2;
    //     memcpy_s(&f2 , sizeof(float) , s , 4);
    //     qDebug()<<f2<<endl;
    //      //32位
    //      s[0]=0xC8;
    //      s[1]=0x06;
    //      s[2]=0xC0;
    //      s[3]=0x40;
    //      float f3;
    //      memcpy_s(&f3 , sizeof(float) , s , 4);
    //     qDebug()<<f3<<endl;

    //     s[0]=0x6B;
    //     s[1]=0x88;
    //     s[2]=0xC6;
    //     s[3]=0xBC;
    //     float f4;
    //     memcpy_s(&f4 , sizeof(float) , s , 4);
    //    qDebug()<<f4<<endl;

    //    s[0]=0xE1;
    //    s[1]=0x07;
    //    s[2]=0xA5;
    //    s[3]=0x44;
    //    float f5;
    //    memcpy_s(&f5 , sizeof(float) , s , 4);
    //   qDebug()<<f5<<endl;

    //   s[0]=0x00;
    //   s[1]=0x00;
    //   s[2]=0x5C;
    //   s[3]=0x43;
    //   float f6;
    //   memcpy_s(&f6 , sizeof(float) , s , 4);
    //  qDebug()<<f6<<endl;

    //  s[0]=0x00;
    //  s[1]=0x00;
    //  s[2]=0xC0;
    //  s[3]=0x40;
    //  float f7;
    //  memcpy_s(&f7 , sizeof(float) , s , 4);
    // qDebug()<<f7<<endl;
    /////////////////////////////////////////////////////////debug
    return -2;//读取到不相等的数据
}

