/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "ad_adjusting.h"

Ad_Adjusting::Ad_Adjusting(QObject *parent) : QThread(parent)
{
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = AdjustConfig::bulid()->item;
}


Ad_Adjusting *Ad_Adjusting::bulid(QObject *parent)
{
    static Ad_Adjusting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Adjusting(parent);
    return sington;
}

bool Ad_Adjusting::sentCmd()
{
    uchar cmd[] = {0x01, 0x04,  0xCC,  0xCC,  0xCC,  0xBB};
    int len = sizeof(cmd);

    cmd[0] = mItem->addr;
    ushort crc = mModbus->rtu_crc(cmd, len-2);
    cmd[len-2] = ((0xff) & crc);
    cmd[len-1] = (crc >> 8);

    bool ret = mModbus->writeSerial(cmd, len);
    if(ret) {
        mPacket->status = tr("发送校准命令！");
    } else {
         mPacket->status = tr("校准发送命令失败！");
    }

    return ret;
}

bool Ad_Adjusting::recvStatus(uchar *buf, int len)
{
    quint8 recvCmd[] ={0x04,  0xCC,  0xCC,  0xCC,  0xBB};

    uchar *ptr = buf;
    bool ret = true;
    if(len == 143) {
        if(*ptr++ == mItem->addr) {
            for(int i=0; i<5; ++i) {
                if(*ptr++ != recvCmd[i])  {
                    ret = false;
                    mPacket->status = tr("校准返回正常！");
                    qDebug() << "AdjustCoreThread resActivation Cmd err";
                }
            }
        }
    } else {
        ret = false;
        mPacket->status = tr("校准返回错误！");
        qDebug() << "AdjustCoreThread resActivation len err";
    }

    return ret;

}


bool Ad_Adjusting::readData()
{
    int count = 1;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    int len = mModbus->readSerial(buf, 1);
    bool ret = false;

    do {
        mPacket->status = tr("正在校准：%1秒").arg(count++);
        len = mModbus->readSerial(buf, 1);
        if(len > 0){
            ret = recvStatus(buf, len);
        }

        if(count > 30) {
             mPacket->status = tr("校准超时！");
             break;
        }
        if(mItem->step == Test_Over) break;

    } while(len <= 0);

    return ret;
}

bool Ad_Adjusting::startAdjust()
{
    bool ret = sentCmd();
    if(ret) {
        ret = readData();
    }

    return ret;
}
