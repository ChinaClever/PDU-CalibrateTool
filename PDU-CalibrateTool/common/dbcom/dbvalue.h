#ifndef DBVALUE_H
#define DBVALUE_H
#include "dbstates.h"

struct sValueItem : public sLogItem{
    QString value;
};

class DbValue : public SqlBasic<sValueItem>
{
    DbValue();
public:
    static DbValue* bulid();
    QString tableName(){return "calibrations";}
    bool insertItem(const sValueItem& item);

protected:
    void createTable();
    bool modifyItem(const sValueItem& item,const QString& cmd);
};

#endif // DBVALUE_H
