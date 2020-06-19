/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_mpduthread.h"

Col_MPduThread::Col_MPduThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_MPduThread *Col_MPduThread::bulid(QObject *parent)
{
    Col_MPduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_MPduThread(parent);
    return sington;
}

bool Col_MPduThread::recvMpduVolCur(uchar *recv, int)
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

bool Col_MPduThread::getMpduVolCur()
{
    bool res = false;
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[20] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        res = recvMpduVolCur(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }

    return res;
}

bool Col_MPduThread::recvMpduEle(uchar *recv, int)
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

int Col_MPduThread::getMpduEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xE1, 0x01, 0x10, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        recvMpduEle(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }
    return ret;
}

bool Col_MPduThread::readPduData()
{
    getMpduEle();
    return getMpduVolCur();
}
