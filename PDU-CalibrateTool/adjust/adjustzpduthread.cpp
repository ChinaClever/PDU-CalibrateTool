/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
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
    qDebug()<<"sendModeCmd" <<endl;
    sendModeCmd();
    qDebug()<<"sendActivateCmd" <<endl;
    sendActivateCmd();
    qDebug()<<"sendGainCmd" <<endl;
    sendGainCmd();

    qDebug()<<"resActivationCmd" <<endl;
    return resActivationCmd();
}

bool AdjustZpduThread::resActivationCmd()
{
    uchar cmd[128] = {0};
    int rtn = 0;//mSerial->read(cmd, 1); // 清空串口数据

    for(int i=0; i<3; ++i) {
        rtn = readSerial(cmd, 5);
        if(cmd[0]!=0x7B)  break;//忽略两个底层发送7B开头的两个命令)
    }

    return resActivateVert(cmd, rtn);
}


int AdjustZpduThread::openAllSwitch()
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

            for(int i=1; i<op-1; ++i) {
                mData->vol[i] = (*ptr++) << 8;
                mData->vol[i] += *ptr++;
            }

            for(int i=0; i<op; ++i) {
                mData->pow[i] = mData->vol[i] * mData->cur[i] * mData->pf[i];
                mData->pow[i] /= (10 * 100);
            }

            ret = true;
        }
    }

    return ret;
}

bool AdjustZpduThread::getZpduVolCur()
{
    bool res = false;
    int k = 6;
    static uchar recv[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};

    cmd[2] = mItem->addr;
    for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44;  //长度
    cmd[k] = getXorNumber(cmd,sizeof(cmd)-1);

    int ret = transmit(cmd, sizeof(cmd), recv, 1);
    if(ret > 0) {
        res = recvZpduVolCur(recv, ret);
    } else {
        qDebug() << "AdjustZpduThread getZpduVolCur err!";
    }

    return res;
}

bool AdjustZpduThread::readPduData()
{
    return getZpduVolCur();
}

void AdjustZpduThread::clearPduEle()
{
    int k = 5;
    static uchar recv[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA4, 0xB4};

    for(int i=0; i<6; i++) cmd[k++] = 0xFF;
    for(int i=0; i<52; i++) cmd[k++] = 0x00;  //预留位
    for(int i=0; i<3; i++) cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = getXorNumber(cmd,sizeof(cmd)-1); //异或校验码
    transmit(cmd, sizeof(cmd), recv, 1);
}
