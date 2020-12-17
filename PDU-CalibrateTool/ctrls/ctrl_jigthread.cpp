/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "ctrl_jigthread.h"

Ctrl_JigThread::Ctrl_JigThread(QObject *parent) : QObject(parent)
{
    mSerial = nullptr;
    QTimer::singleShot(1000,this,SLOT(initSerialSlot()));
}

void Ctrl_JigThread::initSerialSlot()
{
    if(!mSerial) {
        mSerial = Ad_Config::bulid()->item->jig;
        if(mSerial) mSerial->setBaudRate(19200);
    }
}

Ctrl_JigThread *Ctrl_JigThread::bulid(QObject *parent)
{
    Ctrl_JigThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_JigThread(parent);
    return sington;
}

bool Ctrl_JigThread::writeSerial(quint8 *cmd, int len)
{
    initSerialSlot();
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len);
    } else {
        // qDebug() << "Ctrl_JigThread writeSerial err !" << ret;
    }

    return ret;
}

bool Ctrl_JigThread::funSwitch(uchar *on, uchar *off, int f)
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = on[i];  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = off[i];  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;
    cmd[k++] = f; // 统一开关为1

    for(int i=0; i<3; i++)  cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = Ad_Modbus::bulid(this)->getXorNumber(cmd,sizeof(cmd)-1);

    return writeSerial(cmd, sizeof(cmd));
}

bool Ctrl_JigThread::openAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    return funSwitch(on, off, 1);
}

bool Ctrl_JigThread::closeAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0xff;  //关闭有效位

    return funSwitch(on, off, 1);
}

bool Ctrl_JigThread::open()
{
    return openAllSwitch();
}

bool Ctrl_JigThread::down()
{
    return closeAllSwitch();
}
