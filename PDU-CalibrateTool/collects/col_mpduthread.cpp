/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_mpduthread.h"

Col_MpduThread::Col_MpduThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_MpduThread *Col_MpduThread::bulid(QObject *parent)
{
    Col_MpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_MpduThread(parent);
    return sington;
}

bool Col_MpduThread::recvMpduVolCur(uchar *recv, int)
{
    bool ret = false;
    uchar *ptr = recv;
    if(*ptr++ == 0xc1) {
        if(*ptr++ == mItem->addr) {
            mData->size = 8; ptr++;
            uchar sw = *ptr++; // 开关状态 1表示开，0表示关
            for(int i=0; i<mData->size; ++i) {
                mData->sw[i] =  (sw >> (7-i)) & 1;
            }

            ptr = toShort(ptr, mData->size, mData->cur);
            ushort vol = getShort(ptr); ptr += 2;
            for(int i=0; i<mData->size/2; ++i) mData->vol[i] = vol;
            vol = getShort(ptr); ptr += 2;
            for(int i=mData->size/2; i<mData->size; ++i) mData->vol[i] = vol;
            ptr = toShort(ptr, mData->size, mData->pow);

            ret = true;
        } else {
            qDebug() << "AdjustCoreThread recvMpduVolCur addr err!";
        }
    } else {
        qDebug() << "AdjustCoreThread recvMpduVolCur err!";
    }

    return ret;
}

bool Col_MpduThread::getMpduVolCur()
{
    bool res = false;
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[20] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvMpduVolCur(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }

    return res;
}

bool Col_MpduThread::recvMpduEle(uchar *recv, int)
{
    bool ret = true;
    uchar *ptr = recv;
    if(*ptr++ == 0xE1){
        if(*ptr++ == mItem->addr) {
            ptr = toOutputEle(ptr, mData->size, mData->ele);
        } else {
            ret = false;
        }
    } else {
        qDebug() << "AdjustCoreThread recvMpduEle res err!" << *ptr;
    }
    return ret;
}

int Col_MpduThread::getMpduEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xE1, 0x01, 0x10, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        recvMpduEle(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }
    return ret;
}

bool Col_MpduThread::readPduData()
{
    mData->rate = 10;
    bool ret = getMpduVolCur();
    if(ret) {
        ret = getMpduEle();
    }
    return ret;
}
