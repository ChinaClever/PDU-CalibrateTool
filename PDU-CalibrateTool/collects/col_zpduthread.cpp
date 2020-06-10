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

            mData->vol[0] = (ptr[0] << 8) + ptr[1]; ptr += 2;
            mData->vol[mData->size-1] = (ptr[0] << 8) + ptr[1]; ptr += 2;
            ushort sw = (ptr[0] << 8) + ptr[1]; ptr += 2; // 开关状态 1表示开，0表示关
            for(int i=0; i<op; ++i) {
                mData->sw[i] = (sw >> (op-i)) & 1;
            }

            for(int i=0; i<op; ++i) {
                mData->cur[i] = (*ptr++) << 8;
                mData->cur[i] += *ptr++;
            }

            for(int i=0; i<op; ++i) mData->pf[i] = *ptr++;
            for(int i=0; i<op; ++i) {
                mData->ele[i] = (*ptr++) << 16;
                mData->ele[i] += (*ptr++) << 8;
                mData->ele[i] += *ptr++;
            }
            ptr += 3; //忽略三位97
            mData->version = *ptr++;
            mData->chipStatus = *ptr++; // 01表示执行版计量芯片模块损坏，00表示正常。
            ptr++;

            for(int i=1; i<mData->size-1; ++i) {
                mData->vol[i] = (*ptr++) << 8;
                mData->vol[i] += *ptr++;
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

    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        res = recvZpduVolCur(recv, ret);
    } else {
        qDebug() << "Col_ZPduThread getZpduVolCur err!";
    }

    return res;
}

bool Col_ZPduThread::readPduData()
{
    return getZpduVolCur();
}