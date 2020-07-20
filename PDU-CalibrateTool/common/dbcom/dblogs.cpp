#include "dblogs.h"

DbLogs::DbLogs()
{
    createTable();
    tableTile = tr("校准操作日志");
    //hiddens <<  9;
    headList << tr("设备类型") << tr("操作员") << tr("序列码") << tr("校准状态") ;
}


void DbLogs::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "dev            VCHAR,"
            "user           VCHAR,"
            "sn             VCHAR not null,"
            "status         VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbLogs *DbLogs::bulid()
{
    static DbLogs* sington = nullptr;
    if(sington == nullptr)
        sington = new DbLogs();
    return sington;
}

bool DbLogs::insertItem(sLogItem &item)
{
    QString cmd = "insert into %1 (date,time,dev,user,sn,status) "
                  "values(:date,:time,:dev,:user,:sn,:status)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret) emit itemChanged(item.id,Insert);
    return ret;
}


bool DbLogs::modifyItem(const sLogItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":dev",item.dev);
    query.bindValue(":user",item.user);
    query.bindValue(":sn",item.sn);
    query.bindValue(":status",item.status);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}
