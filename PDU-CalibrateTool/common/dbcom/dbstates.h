#ifndef DBSTATUS_H
#define DBSTATUS_H

#include "dbmacs.h"

struct sStateItem : public sLogItem{
    QString memo;
};

class DbStates : public SqlBasic<sStateItem>
{
    DbStates();
public:
    static DbStates* bulid();
    QString tableName(){return "cali_states";}
    bool insertItem(const sStateItem& item);

protected:
    void createTable();
    bool modifyItem(const sStateItem& item,const QString& cmd);
};

#endif // DBSTATUS_H
