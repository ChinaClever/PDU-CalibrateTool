/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"

sDataPacket::sDataPacket()
{
    tg = new sTgObjData;
    data = new sDataUnit;
    devType = new sDevType;
    clear();
    devType->specs = 0;
    devType->devType = 0;
}

void sDataPacket::clear()
{
    uchar sp = devType->specs;
    uchar dt = devType->devType;
    memset(tg, 0, sizeof(sTgObjData));
    memset(data, 0, sizeof(sDataUnit));
    memset(devType, 0, sizeof(sDevType));

    devType->devType = dt;
    devType->specs = sp;
    dev_type.clear();
    status.clear();
    sn.clear();
}


sDataPacket *sDataPacket::bulid()
{
    static sDataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new sDataPacket();
    return sington;
}

int sDataPacket::tgCur()
{
    int ret = 0;
    for(int i=0; i<data->size; ++i) {
        ret += data->cur[i];
    }

    return ret;
}
