/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_adjusting.h"

Ad_Adjusting::Ad_Adjusting(QObject *parent) : QThread(parent)
{
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = Ad_Config::bulid()->item;
}


Ad_Adjusting *Ad_Adjusting::bulid(QObject *parent)
{
    static Ad_Adjusting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Adjusting(parent);
    return sington;
}

bool Ad_Adjusting::transmit(uchar *buf, int len)
{
    bool ret = true;
    uchar recv[64] = {0};
    len = mModbus->transmit(buf, len, recv, 1);
    if(len > 0) {
        ret = recvStatus(recv, len);
    }

    return ret;
}

bool Ad_Adjusting::writeCmd(uchar fn, uchar line)
{
    uchar cmd[] = {0x7B, 0x00, 0xA0, 0x00, 0x66, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = mItem->addr;
    cmd[2] = fn;
    cmd[3] = line;

    ushort crc = mModbus->rtu_crc(cmd, len-2);
    cmd[len-2] = ((0xff) & crc);
    cmd[len-1] = (crc >> 8);

    return transmit(cmd, len);
}

bool Ad_Adjusting::sentCmd()
{
    bool ret = writeCmd(0xA0, 0);
    if(!ret) return ret;

    sDevType *dt = mPacket->devType;
    if(DC == dt->ac) {
        ret = writeCmd(0xA1, 0);
    }

    return writeCmd(0xA2, 0);
}

bool Ad_Adjusting::updateStatus(ushort status)
{
    bool ret = true;

    if(0x1100 == status) {
        mPacket->status = tr("校准返回正常！");
    } else if(0x1101 == status) {
        mPacket->status = tr("校准失败");
        mItem->step = Test_End;
        ret = false;
    } else if(0x1102 == status) {
        mPacket->status = tr("校准解锁成功");
    } else if(status <= 0x1104) {
        mPacket->status = tr("正在校准，%1相 ").arg(status-0x1101);
    } else if(status <= 0x1117) {
        mPacket->status = tr("校准失败：%1相 ").arg(status-0x1114);
        mItem->step = Test_End;
        ret = false;
    } else if(status <= 0x112F) {
        mPacket->status = tr("正在校准，输出位%1 ").arg(status-0x1120);
    } else if(status <= 0x114F) {
        mPacket->status = tr("校准失败：输出位%1 ").arg(status-0x1140);
        mItem->step = Test_End;
        ret = false;
    } else {
        ret = false;
        qDebug() << "Adjust res status err" << status;
    }

    return ret;
}

bool Ad_Adjusting::recvStatus(uchar *recv, int len)
{
    bool ret = true;
    if(len == 8) {
        ushort status = recv[4]*256 + recv[5];
        ret = updateStatus(status);
    } else {
        ret = false;
        qDebug() << "Adjust res len err" << len;
    }

    return ret;
}

bool Ad_Adjusting::readData()
{
    int count = 1;
    bool ret = false;
    uchar *ptr = nullptr;
    uchar buf[MODBUS_RTU_SIZE] = {0};

    do {
        mPacket->status = tr("正在校准：%1秒").arg(count++);
        int len = mModbus->readSerial(buf, 15);
        if(len > 0){
            if(len > 8) {
                ptr = &(buf[len-8]);
                len = 8;
            } else {
                ptr = buf;
            }
            ret = recvStatus(ptr, len);
        } else {
            mItem->step = Test_End;
            mPacket->status = tr("校准超时！");
            break;
        }
        if(mItem->step > Test_vert) {
            mPacket->pass = Test_Fail;
            break;
        }
        qDebug()<<"while(false == ret);"<<ret<<endl;
    } while(false == ret);

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
