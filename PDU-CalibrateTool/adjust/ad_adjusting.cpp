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
    len = mModbus->transmit(buf, len, recv, 2);
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

    if(mPacket->devType->devType == APDU) cmd[1] = 0;//A系列校准时，用地址0，读取设备时用0x04
    ushort crc = mModbus->rtu_crc(cmd, len-2);
    cmd[len-2] = ((0xff) & crc);
    cmd[len-1] = (crc >> 8);

    return transmit(cmd, len);
}

bool Ad_Adjusting::waitDcRecv()
{
    bool ret = false;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    mPacket->status = tr("正在等待直流偏移：请等待...");

    int len = readSerial(buf, 60);
    if(len > 0){
        ret = recvStatus(buf, len);
    } else {
        ret = overWork(tr("直流偏移等待超时！"));
    }

    return ret;
}

bool Ad_Adjusting::writeOffset()
{
    bool ret = true;
    sDevType *dt = mPacket->devType;
    if(DC == dt->ac) {
        mPacket->status = tr("发送直流偏移命令！");
        ret = writeCmd(0xA1, 0);
        if(dt->devType == IP_PDU) ret = waitDcRecv();
        else ret = mModbus->delay(15);
        if(!ret) return ret;

        mPacket->status = tr("设置标准源电流6A");
        ret = mSource->setCur(60);
        if(ret) ret = mModbus->delay(10);
        if(!ret) return ret;
    } else {
        ret = mModbus->delay(1);//15
    }

    return ret;
}

bool Ad_Adjusting::writePhase()
{
    bool ret = mModbus->delay(10);
    if(!ret) return ret;

    sDevType *dt = mPacket->devType;
    if(AC == dt->ac) {
        if(dt->devType == IP_PDU || dt->devType == BM_PDU|| dt->devType == SI_PDU){
            mPacket->status = tr("发送电流电压相位校准命令！");
            ret = writeCmd(0xA3, 60);
            if(ret) ret = mModbus->delay(10);
            if(!ret) return ret;
        }
    }

    return ret;
}

bool Ad_Adjusting::sentCmd()
{
    mPacket->status = tr("即将开始校准！");
    mModbus->delay(2);

    mPacket->status = tr("发送校准解锁命令！");
    bool ret = writeCmd(0xA0, 0);
    if(!ret){
        mModbus->delay(4);
        ret = writeCmd(0xA0, 0);  // 重复发一次命令
        if(!ret) return ret;
    }

    if(ret) ret = writeOffset();
    if(!ret) return ret;

    mPacket->status = tr("发送启动校准命令！");
    ret = writeCmd(0xA2, 0);
    //ret = writePhase();

    return ret;
}

bool Ad_Adjusting::updateStatus(ushort status)
{
    QString str;

    if(0x1100 == status) {
        uchar step = Test_vert;
        mPacket->status = tr("校准返回正常！");
        if(mItem->aiMode) step = Test_End; mItem->step = step;
    } else if(0x1101 == status) {
        str = tr("校准失败");
        mItem->step = Test_vert;
    } else if(0x1102 == status) {
        mPacket->status = tr("校准解锁成功");
    } else if(0x1108 == status) {
        mPacket->status = tr("准直流偏移校准成功");
    }else if(0x1109 == status) {
        str = tr("直流偏移校准失败");
    }else if(0x110A == status) {
        mPacket->status = tr("直流正在校准");
    }else if(0x110B == status) {
        str = tr("直流电流校准失败");
    }else if(0x110C == status) {
        str = tr("直流电压校准失败");
    }else if(status <= 0x1115) {
        if(status%3 == 0)
            mPacket->status = tr("L%1相， 正在校准").arg((status-0x110D)/3+1);
        else if(status%3 == 1)
            mPacket->status = tr("L%1相， 校准成功").arg((status-0x110D)/3+1);
        else if(status%3 == 2)
            mPacket->status = tr("L%1相， 校准失败").arg((status-0x110D)/3+1);
    } else if(status <= 0x1118) {
        str = tr("校准失败：L%1相电流 ").arg(status-0x1115);
    } else if(status <= 0x111C) {
        str = tr("校准失败：L%1相电压 ").arg(status-0x1119);
    }else if(status <= 0x112F) {
        mPacket->status = tr("校准完成，输出位%1 ").arg(status-0x1120);
    } else if(status <= 0x114F) {
        str = tr("电流校准失败：输出位%1").arg(status-0x1140);
    } else if(status <= 0x116F) {
        str = tr("电压校准失败：输出位%1").arg(status-0x1160);
    } else {
        str = tr("校准失败：状态返回错误%1 ").arg(QString::number(status, 16));
    }

    if(str.size()) {
        //ret = overWork(str);
        //mItem->step = Test_End;
        mPacket->status = str;
        mPacket->pass = Test_Fail;
        mModbus->appendLogItem(false);
    } else {
        mPacket->pass = Test_Success;
    }

    return true;
}

bool Ad_Adjusting::recvStatus(uchar *recv, int len)
{
    bool ret = true;
    if((len>0) && (len%8 == 0)) {
        for(int i = 0 ; i < len ; i+=8) {
            ushort status = recv[i+4]*256 + recv[i+5];
            ret = updateStatus(status);
        }
    } else {
        //ret = false;
        qDebug() << "Adjust res len err" << len;
    }

    return ret;
}

int Ad_Adjusting::readSerial(uchar *recv, int sec)
{
    uchar *ptr = nullptr;
    uchar buf[MODBUS_RTU_SIZE] = {0};

    int len = mModbus->readSerial(buf, sec);
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
            ret = overWork(tr("校准超时！")); break;
        }
        if(mItem->step >= Test_vert) break;
    } while(true == ret);

    return ret;
}

bool Ad_Adjusting::startAdjust(YC_StandSource *source)
{
    mSource = source;
    mItem->step = Test_Ading;
    bool ret = sentCmd();
    if(ret && (mItem->step == Test_Ading)){
        ret = readData();
    }

    return ret;
}
