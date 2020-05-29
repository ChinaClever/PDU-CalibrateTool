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
    mSource = StandardSource::bulid(this);
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
    /////////////////////////////////////////////////////////debug
    QByteArray array;
    for(int i = 0; i<len ; i++)
    {
        array.append(*(cmd + i));
    }
    qDebug()  << "[" << array.toHex() << "]"<<endl;
    /////////////////////////////////////////////////////////debug
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len); msleep(100);
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
    if(len == 143) {
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
    writeSerial(cmd, len); delay(2);
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
    writeSerial(cmd, len); delay(5);
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
    writeSerial(cmd, len); delay(5);
}



void AdjustCoreThread::openAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    funSwitch(on, off, 1);
}

void AdjustCoreThread::setBitControl(int id, uchar *buf)
{
    int index = (mData->size * (mItem->addr-1) +id) / 8;
    int key = (mData->size * (mItem->addr-1) +id) % 8 ;
    buf[index] |= 0x80 >> key;
}

void AdjustCoreThread::openOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, on);
        funSwitch(on, off, 0);
    }
}

void AdjustCoreThread::openOnlySwitch(int id)
{
    openOutputSwitch(id);
    delay(1);
    closeOtherSwitch(id);
}

void AdjustCoreThread::closeOtherSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, off);
        for(int i=0; i<6; i++)  off[i] = ~off[i];
        funSwitch(on, off, 0);
    }
}

void AdjustCoreThread::closeAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0xff;  //关闭有效位

    funSwitch(on, off, 1);
}


void AdjustCoreThread::closeOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    if(id > 0) {
        setBitControl(id, off);
        funSwitch(on, off, 0);
    }
}


void AdjustCoreThread::clearAllEle()
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0xFF;
    funClearEle(cmd);
}



void AdjustCoreThread::setClearEle(int id)
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0;

    setBitControl(id, cmd);
    funClearEle(cmd);
}


quint8 AdjustCoreThread::getXorNumber(quint8 *data,int len)
{
    quint8 xorNumber = 0;
    for(int i = 0;i<len ;i++) {
        xorNumber ^= *(data+i);
    }

    return xorNumber;
}



bool AdjustCoreThread::curAllAdjust()
{
    int cur = 0;
    for(int i=0; i<mData->size; ++i) {
        cur += mData->cur[i];
    }

    double min = mItem->cur - mItem->curErr;
    double max = mItem->cur + mItem->curErr;
    min *= mData->rate * COM_RATE_CUR;
    max *= mData->rate * COM_RATE_CUR;
    if((cur >= min) && (cur <= max)) {
        return true;
    }

    return false;
}


bool AdjustCoreThread::curErrRange(int i)
{
    bool ret = true;
    double min = mItem->cur - mItem->curErr;
    double max = mItem->cur + mItem->curErr;
    min *= mData->rate * COM_RATE_CUR;
    max *= mData->rate * COM_RATE_CUR;

    int cur = mData->cur[i] ;
    if((cur >= min) && (cur <= max)) {
        mData->cured[i] = mData->cur[i];
        mData->status[i] = 1;
    } else {
        mData->status[i] = 2;
        ret = false;
    }

    return ret;
}

bool AdjustCoreThread::curOneAdjust()
{
    bool ret = true;

    for(int i=0; i<mData->size; ++i) {
        if(mData->cured[i] < 1) {
            bool res = curErrRange(i);
            if(!res)  ret = false;
        }
    }

    return ret;
}

bool AdjustCoreThread::volAdjust()
{
    int min = mItem->vol - mItem->volErr;
    int max = mItem->vol + mItem->volErr;
    min *= mData->rate;
    max *= mData->rate;

    bool ret = true;
    for(int i=0; i<mData->size; ++i) {
        int vol = mData->vol[i];
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

    if(mItem->vert) {
        ret = curAllAdjust();
    } else {
        ret = curOneAdjust();
    }
    resTgData(mPacket->tg);

    return ret;
}

void AdjustCoreThread::changeSwitch()
{
    for(int k=1; k<=mData->size; ++k) {
        mPacket->status = tr("校验数据\n 第%1输出位").arg(k);
        openOnlySwitch(k); delay(5);

        for(int i=0; i<5; ++i) {
            readPduData();
            if(curErrRange(k)) break;
            delay(1);
        }
    }
}

bool AdjustCoreThread::pduAdjust()
{
    bool ret = true;
    for(int i=0; i<4; ++i) {
        if(mItem->mode || mItem->vert) { // 互感器
            mPacket->status = tr("校验数据\n 第%1次").arg(i+1);
            readPduData();
        } else {
            changeSwitch();
            i = 4; // 不再循环
        }

        ret = dataAdjust();
        if(ret) {
            clearAllEle();
            openAllSwitch();
            break;
        } else {
            ret = delay(2);
            if(!ret) break;
        }
    }

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
    delay(1);
}

/**
 * @brief 校准工作流程
 */
void AdjustCoreThread::workDown()
{
    mPacket->pass = 0;
    mPacket->status = tr("正在校准");
    //mSource->powerReset();

    bool ret = startActivationCmd();
    if(ret) { // 校准成功
        mPacket->status = tr("等待重启!"); delay(3);
        mSource->powerReset(); delay(1);
        openAllSwitch(); delay(4);
        ret = pduAdjust();
    }

    workResult(ret);
    //mSource->powerDown();
}

/**
 * @brief 数据采集工作流程
 */
void AdjustCoreThread::collectData()
{    
    mPacket->pass = 0;
    mPacket->status = tr("数据采集");

    while(mItem->step != Test_Over) {
        readPduData();
        delay(2);
        pduAdjust();
    }
}

void AdjustCoreThread::run()
{
    if(!isRun) {
        isRun = true;
        mPacket->clear();

        if(Collect_Start == mItem->step) {
            collectData();
        } else {
            workDown();
        }
        isRun = false;
    } else {
        qDebug() << "AdjustCoreThread run err" << isRun;
    }
}
