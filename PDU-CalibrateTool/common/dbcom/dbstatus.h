#ifndef DBSTATUS_H
#define DBSTATUS_H

#include "dblogs.h"

class DbStatus : public DbLogs
{
    DbStatus();
public:
    static DbStatus* bulid();
    QString tableName(){return "states";}
};

#endif // DBSTATUS_H
