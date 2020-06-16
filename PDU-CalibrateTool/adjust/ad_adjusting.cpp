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
    mItem = Ad_Config::bulid()->item;
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
    uchar cmd[] = {0x7B, 0x00, 0xA1, 0x00, 0x66, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = mItem->addr;
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

bool Ad_Adjusting::updateStatus(uchar status)
{
    bool ret = true;
    if(0x10 == status) {
        mPacket->status = tr("校准返回正常！");
    } else if(status > 0x20) {
        mItem->step = Test_End;
        mPacket->status = tr("校准失败，相/输出位 %1 出错").arg(status-0x20);
    } else if(status > 0x10) {
        mPacket->status = tr("正在校准，相/输出位 %1 ").arg(status-0x10);
        ret = false;
    } else {
        ret = false;
        qDebug() << "Adjust res status err";
    }

    return ret;
}

bool Ad_Adjusting::recvStatus(uchar *buf, int len)
{
    bool ret = true;
    if(len == 7) {
        ret = updateStatus(buf[4]);
    } else {
        ret = false;
        qDebug() << "Adjust res len err";
    }

    return ret;

}

bool Ad_Adjusting::readData()
{
    int count = 1;
    uchar buf[32] = {0};
    int len = mModbus->readSerial(buf, 1);
    bool ret = false;

    do {
        mPacket->status = tr("正在校准：%1秒").arg(count++);
        len = mModbus->readSerial(buf, 10);
        if(len > 0){
            ret = recvStatus(buf, len);
        } else {
            mItem->step = Test_End;
            mPacket->status = tr("校准超时！");
            break;
        }
        if(mItem->step > Test_vert) {
            mPacket->pass = 2;
            break;
        }

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
