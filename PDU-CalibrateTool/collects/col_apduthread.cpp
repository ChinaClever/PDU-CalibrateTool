/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "col_apduthread.h"

Col_ApduThread::Col_ApduThread(QObject *parent) : Col_CoreThread(parent)
{

}


Col_ApduThread *Col_ApduThread::bulid(QObject *parent)
{
    Col_ApduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Col_ApduThread(parent);
    return sington;
}

bool Col_ApduThread::recvApduVolCur(uchar *recv, int len)
{
    bool ret = false;
    uchar *ptr = recv;
    int index = 62;
    if( mDt->devBothType == IP_PDU_APDU) index = 69;

    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1) && (len == index)) {
    //if((*ptr++ == 0x7B) && (*ptr++ == 0xC1)) {
        if(*ptr++ == mItem->addr) {
//            mData->size = *ptr++;
            mData->hz = *ptr++;

            for(int i = 0 ;i < 6 ; i++)
            {
                mData->vol[i] = getShort(ptr); ptr += 2;
            }
            for(int i = 0 ;i < 6 ; i++)
            {
                mData->cur[i] = getShort(ptr); ptr += 2;
            }
            for(int i = 0 ;i < 6 ; i++)
            {
                mData->pow[i] = getShort(ptr)*mData->rate; ptr += 2;
            }


            toOutputEle(ptr , 6 , mData->ele);
            uchar sw = *ptr++; // 开关状态 1表示开，0表示关
            for(int i=0; i<6; ++i)  mData->sw[i] = (sw >> (6-i)) & 1;
            mData->version = *ptr++;
            ptr++;

            ret = true;
        }
    }

    return ret;
}

bool Col_ApduThread::getApduVolCur()
{
    bool res = false;
    int k = 5;
    static uchar recv[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x04, 0xA9, 0xB9};

    cmd[2] = mItem->addr;
    for(int i=1; i<62; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44;  //长度
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvApduVolCur(recv, ret);
    } else {
        qDebug() << "Col_APduThread getApduVolCur err!" << res;
    }

    return res;
}

bool Col_ApduThread::readPduData()
{
    mData->rate = 10;
    return getApduVolCur();
}
