#include "adjustzpduthread.h"

AdjustZpduThread::AdjustZpduThread(QObject *parent) : AdjustCoreThread(parent)
{

}

AdjustZpduThread *AdjustZpduThread::bulid(QObject *parent)
{
    AdjustZpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustZpduThread(parent);
    return sington;
}

bool AdjustZpduThread::startActivationCmd()
{
    sendModeCmd();
    sendActivationCmd();
    sendGainCmd();

    return resActivationCmd();
}

bool AdjustZpduThread::resActivationCmd()
{
    uchar cmd[128] = {0};
    int rtn = mSerial->read(cmd, 1); // 清空串口数据

    for(int i=0; i<3; ++i) {
        rtn = readSerial(cmd, 60);
        if(data[0]!=0x7B)  break;//忽略两个底层发送7B开头的两个命令)
    }

    return resActivateVert(cmd, rtn);
}


int AdjustZpduThread::openSwitch()
{
    int k = 5;
    static uchar buf[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = 0x00;  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;
    cmd[k++] = 0x01;

    for(int i=0; i<3; i++)  cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = getXorNumber(cmd,sizeof(cmd)-1);

    return transmit(cmd, sizeof(cmd), buf, 1);
}

bool AdjustZpduThread::recvZpduVolCur(uchar *recv, int len)
{
    uchar k=0, *ptr = recv;

    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1) && (len == 127)) {
         if(*ptr++ == mItem->addr) {
                mData->size = *ptr++;
                mData->hz = *ptr++;



         }

    }

}
