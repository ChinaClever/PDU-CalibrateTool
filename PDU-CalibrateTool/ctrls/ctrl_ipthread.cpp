/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_ipthread.h"

Ctrl_IpThread::Ctrl_IpThread(QObject *parent) : Ctrl_CoreThread(parent)
{

}

Ctrl_IpThread *Ctrl_IpThread::bulid(QObject *parent)
{
    Ctrl_IpThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_IpThread(parent);
    return sington;
}

void Ctrl_IpThread::funSwitch(uchar *on, uchar *off, int f)
{

}

void Ctrl_IpThread::funClearEle(uchar *buf)
{   
}


bool Ctrl_IpThread::sentRtuCmd(ushort reg, ushort value)
{
    sRtuSetItem it;
    it.addr = mItem->addr;
    if(mPacket->devType->version == 3) {
        it.fn = 0x06; //V3 写 0x06
    }
    else{
        it.fn = 0x10;//V1 写 0x10
    }

    it.reg = reg;
    it.data = value;

    mModbus->delay(1);
    return mModbus->rtuWrite(&it);
}

bool Ctrl_IpThread::inputMacAddr(uchar *buf)
{
    bool ret = false;
    char *ptr = mItem->cTh.mac_addr;
    int len = strlen(ptr);
    if(len) {
        Ad_MacAddr *mac = Ad_MacAddr::bulid();
        len = mac->macToChar(ptr, buf);
        if(len > 0) ret = updateMacAddr();
    }

    return ret;
}


bool Ctrl_IpThread::updateMacAddr()
{
    bool ret = true;
    char *ptr = mItem->cTh.mac_addr;
    if(strlen(ptr) > 5) {
        mModbus->writeMac(ptr);
        Ad_MacAddr *mac = Ad_MacAddr::bulid();
        QString s = mac->macAdd(ptr);
        char* ch = s.toLatin1().data();
        strcpy(mItem->cTh.mac_addr, ch);
        Ad_Config::bulid()->setMacAddr(s);
    } else {
        mPacket->status = tr("Mac地址未设置！");
        ret = false;
    }

    return ret;
}

bool Ctrl_IpThread::startProcess()
{
    QProcess process(this);
    process.start("pyweb.exe");
    bool ret = checkNet();
    if(ret) {
        mPacket->status = tr("请等待，正在设置设备参数！");
        process.waitForFinished(60*2000);
        ret = updateMacAddr();
    }
    process.close();

    return ret;
}

bool Ctrl_IpThread::checkNet()
{
    mPacket->status = tr("检测设备网络");
    QString ip = CfgCom::bulid()->read("ip_addr", "192.168.1.163").toString();
    bool ret = cm_checkIp(ip);
    if(ret) {
        mPacket->status = tr(" 正常");
    } else {
        mPacket->status = tr(" 错误");
    }
    mModbus->appendLogItem(ret);

    return ret;
}


bool Ctrl_IpThread::initDev()
{
    bool ret = true;

    if(mPacket->devType->ac == AC) {
        int line = mItem->cTh.ip_lines;
        //if(line > 1) {
            ret = sentRtuCmd(1019, line);
        //}
    }

    return ret;
}

bool Ctrl_IpThread::factorySet()
{
    bool ret = mItem->cTh.repair_en;
    if(!ret) {
        ret = startProcess();
    }

    return ret;
}
