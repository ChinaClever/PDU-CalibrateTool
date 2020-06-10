#ifndef DB_USR_H
#define DB_USR_H
#include "basicsql.h"

struct sUserItem : public DbBasicItem{
    sUserItem():jur(0){}
    int jur;
    QString name,pwd,jurisdiction, email,telephone,remarks;
};



class DbUser : public SqlBasic<sUserItem>
{
    Q_OBJECT
    DbUser();
public:
    static DbUser* bulid();
    QString tableName(){return "users";}

    int contains(const QString &name);
    bool insertItem(sUserItem& item);
    bool updateItem(const sUserItem& item);

    sUserItem selItemsByName(const QString& Name);
    void removeItemsByName(const QString& name);


protected:
    void createTable();
    bool modifyItem(const sUserItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,sUserItem &item);
};

#endif // DB_USR_H
