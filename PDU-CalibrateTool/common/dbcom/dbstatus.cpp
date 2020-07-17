#include "dbstatus.h"

DbStatus::DbStatus()
{

}

DbStatus *DbStatus::bulid()
{
    static DbStatus* sington = nullptr;
    if(sington == nullptr)
        sington = new DbStatus();
    return sington;
}
