#include "ad_autoid.h"

Ad_AutoID::Ad_AutoID(QObject *parent) : QThread(parent)
{        
    mPacket = sDataPacket::bulid();
    mModbus = Ad_Modbus::bulid(this);
    mItem = AdjustConfig::bulid()->item;
}

Ad_AutoID *Ad_AutoID::bulid(QObject *parent)
{
    static Ad_AutoID* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_AutoID(parent);
    return sington;
}

void Ad_AutoID::initReadType(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0xA001;
    it.len = 2;
}

bool Ad_AutoID::analysDevType(uchar *buf, int len)
{
    if(len != 2) return false;

    switch (buf[0]) {
    case 'Z':
        mItem->devType = 1;
        mItem->dev_type = "ZPDU ";
        break;

    case 'M':
        mItem->devType = 2;
        mItem->dev_type = "MPDU ";
        break;

    case 'R':
        mItem->devType = 3;
        mItem->dev_type = "RPDU ";
        break;

    default:
        break;
    }


    switch (buf[1]) {
    case '1':
        mItem->adMode = 1;
        mItem->dev_type += tr("互感器 ");
        break;

    case '2':
        mItem->adMode = 2;
        mItem->dev_type += tr("锰铜 ");
        break;

    default:
        break;
    }

    return true;
}

bool Ad_AutoID::readDevType()
{    
    sRtuItem it;
    initReadType(it);

    uchar recv[6] = {0};
    mPacket->status = tr("正在识别模块类型！");
    int len = mModbus->rtuTrans(&it, recv);
    bool ret = analysDevType(recv, len);
    if(ret) {
        mPacket->status = tr("识别模块：%1").arg(mItem->dev_type);
    } else {
        mPacket->status = tr("模块识别错误！");
    }

    return ret;
}

