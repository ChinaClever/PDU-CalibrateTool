/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "dbuserlog.h"

DbUserLog::DbUserLog()
{
    createTable();
    tableTile = tr("用户日志");
//    hiddens << 1 << 2 << 5 << 9;
    headList << tr("用户名") << tr("描述");
}


void DbUserLog::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "name           VCHAR,"
            "remarks        VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbUserLog *DbUserLog::bulid()
{
    static DbUserLog* sington = nullptr;
    if(sington == nullptr)
        sington = new DbUserLog();
    return sington;
}


bool DbUserLog::insertItem(sUserLogItem &item)
{
    QString cmd = "insert into %1 (date,time,name,remarks) "
                  "values(:date,:time,:name,:remarks)";
    return modifyItem(item,cmd.arg(tableName()));
}


bool DbUserLog::modifyItem(const sUserLogItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":name",item.name);
    query.bindValue(":remarks",item.remarks);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}
