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
    QString str;

    if(0x1100 == status) {
        mItem->step = Test_vert;
        mPacket->status = tr("校准返回正常！");
    } else if(0x1101 == status) {
        str = tr("校准失败");
    } else if(0x1102 == status) {
        mPacket->status = tr("校准解锁成功");
    } else if(status <= 0x1104) {
        mPacket->status = tr("正在校准，%1相 ").arg(status-0x1101);
    } else if(status <= 0x1117) {
        str = tr("校准失败：%1相 ").arg(status-0x1114);
    } else if(status <= 0x112F) {
        mPacket->status = tr("正在校准，输出位%1 ").arg(status-0x1120);
    } else if(status <= 0x114F) {
        str = tr("校准失败：输出位%1 ").arg(status-0x1140);
    } else {
        str = tr("校准失败：状态返回错误%1 ").arg(QString::number(status, 16));
    }

    if(str.size()) {
        ret = overWork(str);
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

int Ad_Adjusting::readSerial(uchar *recv)
{
    uchar *ptr = nullptr;
    uchar buf[MODBUS_RTU_SIZE] = {0};

    int len = mModbus->readSerial(buf, 15);
    if(len > 0){
        if(len > 8) {
            ptr = &(buf[len-8]);
            len = 8;
        } else {
            ptr = buf;
        }

        for(int i=0; i<len; ++i) {
            recv[i] = ptr[i];
        }
    }

    return len;
}

bool Ad_Adjusting::overWork(const QString &str)
{
    mItem->step = Test_End;
    mPacket->pass = Test_Fail;
    mPacket->status = str;
    return false;
}

bool Ad_Adjusting::readData()
{
    bool ret = false;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    mPacket->status = tr("正在校准：请等待...");

    do {
        int len = readSerial(buf);
        if(len > 0){
            ret = recvStatus(buf, len);
        } else {
            ret = overWork(tr("校准超时！"));
            break;
        }
        if(mItem->step >= Test_vert) break;
    } while(true == ret);

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
