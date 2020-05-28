/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "adjustmpduthread.h"

AdjustMpduThread::AdjustMpduThread(QObject *parent) : AdjustCoreThread(parent)
{

}

AdjustMpduThread *AdjustMpduThread::bulid(QObject *parent)
{
    AdjustMpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustMpduThread(parent);
    return sington;
}

bool AdjustMpduThread::resActivationCmd()
{
    uchar cmd[128] = {0};
    int rtn = mSerial->read(cmd, 1); // 清空串口数据
    rtn = readSerial(cmd, 6);

    return resActivateVert(cmd, rtn);
}

bool AdjustMpduThread::startActivationCmd()
{
    sendActivateCmd();
    sendModeCmd();
    //sendGainCmd();

    return resActivationCmd();
}

int AdjustMpduThread::openAllSwitch()
{
    static uchar buf[256] = {0};
    uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xA1,
                   0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9,
                   0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9,
                   0x00, 0x01, 0x00, 0xCC};

    cmd[18] = cmd[2] = mItem->addr;
    cmd[20] = getXorNumber(cmd,sizeof(cmd)-1);

    return transmit(cmd, sizeof(cmd), buf, 1);
}




bool AdjustMpduThread::recvMpduVolCur(uchar *recv, int)
{
    bool ret = false;
    uchar *ptr = recv;
    if(*ptr++ == 0xc1) {
        if(*ptr++ == mItem->addr) {
            mData->size = 8; ptr++;
            uchar sw = *ptr++; // 开关状态 1表示开，0表示关
            for(int i=0; i<8; ++i) {
                mData->sw[i] =  (sw >> (7-i)) & 1;
            }

            for(int i=0; i<8; ++i) {
                mData->cur[i] = (*ptr++) << 8;
                mData->cur[i] += *ptr++;
            }

            ushort vol = (ptr[0] << 8) + ptr[1]; ptr += 2;
            for(int i=0; i<4; ++i) mData->vol[i] = vol;

            vol = (ptr[0] << 8) + ptr[1]; ptr += 2;
            for(int i=4; i<8; ++i) mData->vol[i] = vol;

            for(int i=0; i<8; ++i) {
                mData->pow[i] = (*ptr++) << 8;
                mData->pow[i] += *ptr++;
            }
            ret = true;

        } else {
            qDebug() << "AdjustCoreThread recvMpduVolCur addr err!";
        }
    } else {
        qDebug() << "AdjustCoreThread recvMpduVolCur err!";
    }

    return ret;
}

bool AdjustMpduThread::getMpduVolCur()
{
    bool res = false;
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[20] = getXorNumber(cmd,sizeof(cmd)-1);

    int ret = transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        res = recvMpduVolCur(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }

    return res;
}



bool AdjustMpduThread::recvMpduEle(uchar *recv, int)
{
    bool ret = true;
    uchar *ptr = recv;
    if(*ptr++ == 0xE1){
        if(*ptr++ == mItem->addr) {
            for(int i=0; i<8; ++i) {
                mData->ele[i] = (*ptr++) << 16;
                mData->ele[i] += (*ptr++) << 8;
                mData->ele[i] += *ptr++;
            }
        } else {
            ret = false;
        }
    } else {
        qDebug() << "AdjustCoreThread recvMpduEle res err!" << *ptr;
    }
    return ret;
}

int AdjustMpduThread::getMpduEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xE1, 0x01, 0x10, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[15] = getXorNumber(cmd,sizeof(cmd)-1);

    int ret = transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        recvMpduEle(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }
    return ret;
}

bool AdjustMpduThread::readPduData()
{
    getMpduEle();
    return getMpduVolCur();
}

void AdjustMpduThread::clearPduEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xD1,
                          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[20] = getXorNumber(cmd,sizeof(cmd)-1);
    transmit(cmd, sizeof(cmd), recv, 1);
}
