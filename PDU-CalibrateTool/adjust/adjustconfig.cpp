#include "adjustconfig.h"

AdjustConfig::AdjustConfig()
{
    item = new sConfigItem();
    item->serial = nullptr;
}

AdjustConfig *AdjustConfig::bulid()
{
    static AdjustConfig* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustConfig();
    return sington;
}
