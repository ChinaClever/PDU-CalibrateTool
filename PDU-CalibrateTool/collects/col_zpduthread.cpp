/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_zpduthread.h"

Col_ZPduThread::Col_ZPduThread(QObject *parent) : Col_CoreThread(parent)
{

}


Col_ZPduThread *Col_ZPduThread::bulid(QObject *parent)
{
    Col_ZPduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_ZPduThread(parent);
    return sington;
}

bool Col_ZPduThread::recvZpduVolCur(uchar *recv, int len)
{
    int op = 14;
    bool ret = false;
    uchar *ptr = recv;

    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1) && (len == 127)) {
        if(*ptr++ == mItem->addr) {
            mData->size = *ptr++;
            mData->hz = *ptr++;

            mData->vol[0] = getShort(ptr); ptr += 2;
            mData->vol[mData->size-1] = getShort(ptr); ptr += 2;
            ushort sw = getShort(ptr); ptr += 2; // 开关状态 1表示开，0表示关
            for(int i=0; i<op; ++i) {
                mData->sw[i] = (sw >> (15-i)) & 1;
            }

            ptr = toShort(ptr, op, mData->cur);
            ptr = toChar(ptr, op, mData->pf);
            ptr = toOutputEle(ptr, op, mData->ele);
            ptr += 3; //忽略三位97
            mData->version = *ptr++;
            mData->chipStatus = *ptr++; // 01表示执行版计量芯片模块损坏，00表示正常。
            ptr++;

            for(int i=1; i<mData->size-1; ++i) {
                mData->vol[i] = getShort(ptr); ptr += 2;
            }

            for(int i=0; i<op; ++i) {
                mData->pow[i] = mData->vol[i] * mData->cur[i] * mData->pf[i];
                mData->pow[i] /= (10 * 1000);
            }

            ret = true;
        }
    }

    return ret;
}

bool Col_ZPduThread::getZpduVolCur()
{
    bool res = false;
    int k = 6;
    static uchar recv[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};

    cmd[2] = mItem->addr;
    for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44;  //长度
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvZpduVolCur(recv, ret);
    } else {
        qDebug() << "Col_ZPduThread getZpduVolCur err!";
    }

    return res;
}

bool Col_ZPduThread::readPduData()
{
    mData->rate = 10;
    return getZpduVolCur();
}
