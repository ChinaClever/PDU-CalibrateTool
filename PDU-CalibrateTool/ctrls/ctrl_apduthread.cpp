#include "ctrl_apduthread.h"

Ctrl_ApduThread::Ctrl_ApduThread(QObject *parent): Ctrl_CoreThread(parent)
{

}

Ctrl_ApduThread *Ctrl_ApduThread::bulid(QObject *parent)
{
    Ctrl_ApduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_ApduThread(parent);
    return sington;
}

void Ctrl_ApduThread::funClearEle(uchar *buf)
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA4, 0xB4};

    for(int i=0; i<8; i++) cmd[k++] = 0x00;  //8个预留位
    for(int i=0; i<1; i++) cmd[k++] = buf[i];
    for(int i=0; i<52; i++) cmd[k++] = 0x00;  //预留位
    cmd[k++] = 0x44;
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1); //异或校验码
    mModbus->writeSerial(cmd, sizeof(cmd));
}

