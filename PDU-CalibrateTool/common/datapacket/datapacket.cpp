/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"

sDataPacket::sDataPacket()
{
    tg = new sTgObjData;
    data = new sDataUnit;
}

void sDataPacket::clear()
{
    memset(tg, 0, sizeof(sTgObjData));
    memset(data, 0, sizeof(sDataUnit));
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