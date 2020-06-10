#ifndef DBUSERLANDLOG_H
#define DBUSERLANDLOG_H
#include "db_user.h"
#include "landinguser.h"

extern QString user_land_name();
struct sUserLogItem : public DbBasicItem{
    sUserLogItem(){
        name = user_land_name();
    }
    QString name,remarks;
};

class DbUserLog : public SqlBasic<sUserLogItem>
{
    DbUserLog();
public:
    static DbUserLog* bulid();
    QString tableName(){return "userlogs";}
    bool insertItem(sUserLogItem& item);

protected:
    void createTable();
    bool modifyItem(const sUserLogItem &item, const QString &cmd);
};

#endif // DBUSERLANDLOG_H
