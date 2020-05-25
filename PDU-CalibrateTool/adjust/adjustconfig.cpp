/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "adjustconfig.h"

AdjustConfig::AdjustConfig()
{
    item = new sConfigItem();
    item->serial = nullptr;
    item->source = nullptr;
}

AdjustConfig *AdjustConfig::bulid()
{
    static AdjustConfig* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustConfig();
    return sington;
}
