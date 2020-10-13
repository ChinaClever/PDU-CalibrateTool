#ifndef DBMACS_H
#define DBMACS_H
#include "dblogs.h"

struct sMacItem : public DbBasicItem{
    sMacItem(){}
    QString dev,user,op, sn, mac;
};

class DbMacs : public SqlBasic<sMacItem>
{
    Q_OBJECT
    DbMacs();
public:
    static DbMacs* bulid();
    QString tableName(){return "dev_macs";}

    int contains(const QString &mac);
    bool insertItem(sMacItem& item);
    bool updateItem(const sMacItem& item);

    sMacItem selItemByMac(const QString& mac);
    void removeMac(const QString& mac);

protected:
    void createTable();
    bool modifyItem(const sMacItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sMacItem &item);
};

#endif // DBMACS_H
