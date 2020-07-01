/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "sn_managethread.h"

SN_ManageThread::SN_ManageThread(QObject *parent) : QThread(parent)
{
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = Ad_Config::bulid()->item;
}

SN_ManageThread *SN_ManageThread::bulid(QObject *parent)
{
    SN_ManageThread* sington = nullptr;
    if(sington == nullptr)
        sington = new SN_ManageThread(parent);
    return sington;
}

void SN_ManageThread::initReadCmd(sRtuItem &item)
{
    item.addr = mItem->addr;
    item.fn = 0x03;
    item.reg = 0xA001;
    item.num = 6;
}

bool SN_ManageThread::checkSn(uchar *sn, int len)
{
    bool ret = false;
    if(len == 9) {
        uchar exor = mModbus->getXorNumber(sn, len-1);
        if(exor == sn[len-1]) {
            ret = false;
        }
    }
    return ret;
}

bool SN_ManageThread::analySn(uchar *sn, int len, sSnItem &it)
{
    uchar *ptr = sn;
    for(int i=0; i<4; ++i) {
        it.devType[i] = *ptr++;
    }

    bool ret = checkSn(sn, len);
    if(ret) {
        for(int i=0; i<4; ++i) {
            it.date[i] = *ptr++;
        }
        it.num = (*ptr++) << 8;
        it.num += *ptr++;

        it.pc = *ptr++;
        it.exor = *ptr++;
    }

    return ret;
}

void SN_ManageThread::toSnStr(sSnItem &it)
{
    QString sn  = QString("%1%2%3%4%5%6%7%8%9%10")
            .arg(it.devType[0], 2, 16, QLatin1Char('0'))
            .arg(it.devType[1], 2, 16, QLatin1Char('0'))
            .arg(it.devType[2], 2, 16, QLatin1Char('0'))
            .arg(it.devType[3], 2, 16, QLatin1Char('0'))
            .arg(it.date[1], 2, 10, QLatin1Char('0'))
            .arg(it.date[2], 2, 10, QLatin1Char('0'))
            .arg(it.date[3], 2, 10, QLatin1Char('0'))
            .arg(it.num, 4, 10, QLatin1Char('0'))
            .arg(it.pc, 2, 16, QLatin1Char('0'))
            .arg(it.exor, 2, 16, QLatin1Char('0'));

    it.sn = sn.toUpper();
}

bool SN_ManageThread::readSn(sSnItem &itSn)
{
    sRtuItem itRtu;
    bool ret = false;

    initReadCmd(itRtu);
    uchar buf[32] = {0};
    int len = mModbus->rtuRead(&itRtu, buf);
    if(len > 0) {
        ret =  analySn(buf, len, itSn);
        if(ret) {
            toSnStr(itSn);
        }
    }

    return ret;
}

void SN_ManageThread::initWriteCmd(sRtuSetItem &item, uchar *data, int len)
{
    item.addr = mItem->addr;
    item.fn = 0x10;
    item.reg = 0xA003;
    item.num = 4;
    item.len = len;
    for(int i=0; i<len; ++i) {
        item.data[i] = data[i];
    }
}

void SN_ManageThread::createSn(sSnItem &it)
{
    QDate date = QDate::currentDate();
    it.date[0] = 0;
    it.date[1] = date.year() % 100;
    it.date[2] = date.month();
    it.date[3] = date.day();

    it.num = ++(mItem->currentNum);
    it.pc = mItem->pcNum;
    toSnStr(it);

    Ad_Config::bulid()->setCurrentNum();
}

int SN_ManageThread::toSnData(sSnItem &it, uchar *data)
{
    int k = 0;
    for(int i=0; i<4; ++i)
        data[k++] = it.devType[i];

    for(int i=0; i<4; ++i)
        data[k++] = it.date[i];

    data[k++] = it.num / 256;
    data[k++] = it.num % 256;

    data[k++] = it.pc;

    uchar exor = mModbus->getXorNumber(data, 11);
    data[k++] = it.exor = exor;

    return 12;
}

bool SN_ManageThread::writeSn(sSnItem &itSn)
{
    createSn(itSn);
    uchar buf[32] = {0};
    int len = toSnData(itSn, buf);

    sRtuSetItem itRtu;
    initWriteCmd(itRtu, buf, len);
    mModbus->delay(1);

    return mModbus->rtuWrite(&itRtu);
}

void SN_ManageThread::writeStatus(bool ret)
{
    if(ret) {
        mPacket->status = tr("已写入序列号：\n%1").arg(mSnItem.sn);
    } else {
        mPacket->status = tr("序列号写入失败：\n%1").arg(mSnItem.sn);
        mItem->currentNum -= 1;
    }
}

/**
 * @brief 读取序列号和写入序列号业务逻辑函数
 * @param
 * @return
 */
bool SN_ManageThread::snEnter()
{
    mSnItem.sn.clear();

    bool ret = readSn(mSnItem);
    if(ret) {
        mPacket->status = tr("已读到序列号：\n%1").arg(mSnItem.sn);
    } else {
        ret = writeSn(mSnItem);
        writeStatus(ret);
    }
    mPacket->sn = mSnItem.sn;
    mModbus->delay(1);

    return ret;
}
