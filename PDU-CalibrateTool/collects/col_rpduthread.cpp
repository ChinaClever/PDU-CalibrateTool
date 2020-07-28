/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_rpduthread.h"

Col_RpduThread::Col_RpduThread(QObject *parent) : Col_CoreThread(parent)
{

}

Col_RpduThread *Col_RpduThread::bulid(QObject *parent)
{
    Col_RpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_RpduThread(parent);
    return sington;
}


bool Col_RpduThread::recvVolCur(uchar *recv, int)
{
    bool ret = false;
    uchar *ptr = recv;
    if(*ptr++ == 0xc1) {
        if(*ptr++ == mItem->addr) {
            mData->size = *ptr++;
            for(int i=0; i<mData->size; ++i) {
                mData->cur[i] = *ptr++;
            }
            ptr++; // 预留

            uchar sw = *ptr++; // 开关状态 1表示开，0表示关
            for(int i=0; i<mData->size; ++i) {
                mData->sw[i] =  (sw >> (7-i)) & 1;
            }

            ret = true;
        } else {
            qDebug() << "recv Rpdu VolCur addr err!";
        }
    } else {
        qDebug() << "recv Rpdu err!";
    }

    return ret;
}

bool Col_RpduThread::getVolCur()
{
    bool res = false;
    static uchar recv[256] = {0};
    static uchar cmd[16] = {0x7B, 0xC1, 0x01, 0x10,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvVolCur(recv, ret);
    } else {
        qDebug() << "Col_RpduThread err!";
    }

    return res;
}


bool Col_RpduThread::recvEle(uchar *recv, int)
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
        qDebug() << "recv Rpdu Ele res err!" << *ptr;
    }
    return ret;
}

int Col_RpduThread::getEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[16] = {0x7B, 0xE1, 0x01, 0x10,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mItem->addr;
    cmd[15] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        recvEle(recv, ret);
    } else {
        qDebug() << "Col_RpduThread Ele err!";
    }
    return ret;
}

bool Col_RpduThread::readPduData()
{
    mData->rate = 1;
    bool ret = getVolCur();
    if(ret) {
        ret = getEle();
    }
    return ret;
}
