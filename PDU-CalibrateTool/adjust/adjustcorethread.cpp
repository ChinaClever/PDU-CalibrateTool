/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "adjustcorethread.h"

AdjustCoreThread::AdjustCoreThread(QObject *parent) : QThread(parent)
{
    mPacket =sDataPacket::bulid();
    mItem = AdjustConfig::bulid()->item;
    mSerial = mItem->serial;
    mData = mPacket->data;
    isRun = false;
}


AdjustCoreThread::~AdjustCoreThread()
{
    mItem->step = Test_Over;
    wait();
}


bool AdjustCoreThread::delay(int s)
{
    bool ret = true;
    for(int i=0; i<10*s; ++i) {
        if(mItem->step != Test_Over) {
            msleep(100);
        } else {
            ret = false;
            break;
        }
    }

    return ret;
}

int AdjustCoreThread::readSerial(quint8 *cmd, int sec)
{
    int rtn = 0;
    bool ret = mSerial->isOpened();
    if(ret) {
        for(int i=0; i<sec*2; ++i) {
            if(mItem->step != Test_Over) {
                rtn += mSerial->read(cmd, 5);
            } else {
                return 0;
            }
        }
    }

    return rtn;
}

bool AdjustCoreThread::writeSerial(quint8 *cmd, int len)
{
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len);
    } else {
        qDebug() << "AdjustCoreThread writeSerial err !" << ret;
    }

    return ret;
}


int AdjustCoreThread::transmit(uchar *sent, int len, uchar *recv, int sec)
{
    int rtn = 0;
    bool ret = writeSerial(sent, len);
    if(ret) {
        rtn = readSerial(recv, sec);
    }

    return rtn;
}



bool AdjustCoreThread::resActivateVert(uchar *cmd, int len)
{
    quint8 recvCmd[] ={0x04,  0xCC,  0xCC,  0xCC,  0xBB}; //增益应答Ok

    uchar *ptr = cmd;
    bool ret = true;
    if(len == 7) {
        if(*ptr++ == mItem->addr) {
            for(int i=0; i<5; ++i) {
                if(*ptr++ != recvCmd[i])  {
                    ret = false;
                    qDebug() << "AdjustCoreThread resActivation Cmd err";
                }
            }
        }
    } else {
        ret = false;
        qDebug() << "AdjustCoreThread resActivation len err";
    }

    return ret;
}



void AdjustCoreThread::sendActivateCmd()
{
    int len = 16;
    static quint8 activationCmd[16]={0x7B, 0xA0, 0x10,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xCB};
    quint8 *cmd = activationCmd;
    writeSerial(cmd, len); delay(300);
}


void AdjustCoreThread::sendModeCmd()
{
    int len = 16;
    static quint8 modelCmd[2][4][16]= {
        { //锰铜模式
          {0x7B, 0xA6, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD},
          {0x7B, 0xA6, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE},
          {0x7B, 0xA6, 0x03, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF},
          {0x7B, 0xA6, 0x04, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8}
        },
        { //继电器模式
          {0x7B, 0xA6, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC},
          {0x7B, 0xA6, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF},
          {0x7B, 0xA6, 0x03, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE},
          {0x7B, 0xA6, 0x04, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9}
        }};

    int addr = mItem->addr - 1;
    quint8 *cmd = modelCmd[mItem->mode][addr];
    writeSerial(cmd, len); delay(1300);
}

void AdjustCoreThread::sendGainCmd()
{
    int len = 16;
    static quint8 gainCmd[4][6]={ //增益校准命令
                                  {0x01,  0x04,  0x04,  0x01,  0x04,  0x04 },
                                  {0x02,  0x04,  0x04,  0x01,  0x04,  0x07 },
                                  {0x03,  0x04,  0x04,  0x01,  0x04,  0x06 },
                                  {0x04 , 0x04,  0x04,  0x01,  0x04,  0x01 }
                                };
    int addr = mItem->addr - 1;
    quint8 *cmd = gainCmd[addr];
    writeSerial(cmd, len); delay(1300);
}





quint8 AdjustCoreThread::getXorNumber(quint8 *data,int len)
{
    quint8 xorNumber = 0;
    for(int i = 0;i<len ;i++) {
        xorNumber ^= *(data+i);
    }

    return xorNumber;
}

bool AdjustCoreThread::pduAdjust()
{
    bool ret = true;
    for(int i=0; i<3; ++i) {
        mPacket->status = tr("校验数据\n 第%1次").arg(i+1);
        readPduData();
        ret = dataAdjust();
        if(ret) {
            clearPduEle();
            break;
        } else {
            ret = delay(1);
            if(!ret) break;
        }
    }

    return ret;
}


bool AdjustCoreThread::curAllAdjust()
{
    int cur = 0;
    for(int i=0; i<mData->size; ++i) {
        cur += mData->cur[i];
    }

    int min = mItem->cur - mItem->curErr;
    int max = mItem->cur + mItem->curErr;
    if((cur >= min) && (cur <= max)) {
        return true;
    }

    return false;
}

bool AdjustCoreThread::curOneAdjust()
{
    int min = mItem->cur - mItem->curErr;
    int max = mItem->cur + mItem->curErr;

    bool ret = true;
    for(int i=0; i<mData->size; ++i) {
        int cur = mData->cur[i] / mData->rate;
        if((cur >= min) && (cur <= max)) {
            mData->status[i] = 1;
        } else {
            ret = false;
            mData->status[i] = 2;
        }
    }

    return ret;
}

bool AdjustCoreThread::volAdjust()
{
    int min = mItem->vol - mItem->volErr;
    int max = mItem->vol + mItem->volErr;

    bool ret = true;
    for(int i=0; i<mData->size; ++i) {
        int vol = mData->vol[i] * mData->rate;
        if((vol >= min) && (vol <= max)) {
            mData->status[i] = 1;
        } else {
            ret = false;
            mData->status[i] = 2;
        }
    }

    return ret;
}

void AdjustCoreThread::resTgData(sTgObjData *tg)
{
    for(int i=0; i<mData->size; ++i) {
        tg->cur += mData->cur[i];
        tg->pow += mData->pow[i];
    }
}

bool AdjustCoreThread::dataAdjust()
{
    bool ret = volAdjust();
    if(!ret) return ret;

    if(mItem->mode) {
        ret = curOneAdjust();
    } else {
        ret = curAllAdjust();
    }
    resTgData(mPacket->tg);

    return ret;
}

void AdjustCoreThread::workResult(bool res)
{
    QString str = tr("校准失败!");
    if(res) {
        mPacket->pass = 1;
        str = tr("校准成功!");
    } else {
        mPacket->pass = 2;
    }
    mPacket->status = str;
    mItem->mode = Test_End;
}


void AdjustCoreThread::workDown()
{
    mPacket->pass = 0;
    mPacket->status = tr("正在校准");
    bool ret = startActivationCmd();
    if(ret) { // 校准成功
        mPacket->status = tr("等待重启!"); delay(1300);
        openAllSwitch();
        ret = pduAdjust();
    }

    workResult(ret);
}

void AdjustCoreThread::run()
{
    if(!isRun) {
        isRun = true;
        workDown();
        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}