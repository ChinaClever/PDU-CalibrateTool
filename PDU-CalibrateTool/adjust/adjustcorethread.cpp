#include "adjustcorethread.h"

AdjustCoreThread::AdjustCoreThread(QObject *parent) : QThread(parent)
{
    mPacket =sDataPacket::bulid();
    mItem = AdjustConfig::bulid()->item;
    mSerial = mItem->serial;
    mData = mPacket->data;
}


AdjustCoreThread::~AdjustCoreThread()
{
    mItem->mode = Test_Over;
    wait();
}

AdjustCoreThread *AdjustCoreThread::bulid(QObject *parent)
{
    static AdjustCoreThread* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustCoreThread(parent);
    return sington;
}

void AdjustCoreThread::delay(int s)
{
    for(int i=0; i<10*s; ++i) {
        if(mItem->mode != Test_Over) msleep(100);
    }
}

bool AdjustCoreThread::writeSerial(quint8 *cmd, int len)
{
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len);
    } else {
        qDebug() << "AdjustCoreThread writeSerial err !";
    }

    return ret;
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


bool AdjustCoreThread::resActivationCmd()
{
    static uchar cmd[128] = {0};
    int rtn = mSerial->read(cmd, 5); // 清空串口数据
    rtn = mSerial->read(cmd, 600);

    return resActivateVert(cmd, rtn);
}


bool AdjustCoreThread::startActivationCmd()
{
    int len = 16;
    quint8 activationCmd[16]={0x7B, 0xA0, 0x10,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0xCB};
    quint8 *cmd = activationCmd;
    bool ret = writeSerial(cmd, len); delay(300);

    quint8 modelCmd[2][4][16]= {
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

    cmd = modelCmd[mItem->mode][mItem->addr];
    ret = writeSerial(cmd, len); delay(1300);

    quint8 gainCmd[4][6]={ //增益校准命令
        {0x01,  0x04,  0x04,  0x01,  0x04,  0x04 },
        {0x02,  0x04,  0x04,  0x01,  0x04,  0x07 },
        {0x03,  0x04,  0x04,  0x01,  0x04,  0x06 },
        {0x04 , 0x04,  0x04,  0x01,  0x04,  0x01 }
    };
    cmd = gainCmd[mItem->addr];
    ret = writeSerial(cmd, len); delay(1300);

    return resActivationCmd();
}


void AdjustCoreThread::workDown()
{
    mPacket->pass = 0;
    mPacket->status = tr("正在校准");
    bool ret = startActivationCmd();
    if(ret) { // 校准成功
        mPacket->status = tr("等待重启!"); delay(1300);


        ////// 赢取数据






    } else { // 校准失败
        mPacket->pass = 2;
        mPacket->status = tr("校准失败");
    }


}

void AdjustCoreThread::run()
{

}
