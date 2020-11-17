/*
 *
 *
 *  Created on: 2020年10月11日
 *      Author: Lzy
 */
#include "dbstates.h"

DbStates::DbStates()
{
    createTable();
    tableTile = tr("校准过程日志");
    //hiddens <<  9;
    headList << tr("设备类型") << tr("客户名称") << tr("操作员") << tr("设备序列号") << tr("状态") << tr("内容");
}

void DbStates::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "dev            VCHAR,"
            "user           VCHAR,"
            "op             VCHAR,"
            "sn             VCHAR not null,"
            "result         VCHAR,"
            "memo           VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbStates *DbStates::bulid()
{
    static DbStates* sington = nullptr;
    if(sington == nullptr)
        sington = new DbStates();
    return sington;
}

bool DbStates::insertItem(const sStateItem &item)
{
    QString cmd = "insert into %1 (date,time,dev,user,sn,op,result,memo) "
                  "values(:date,:time,:dev,:user,:op,:sn,:result,:memo)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    //if(ret) emit itemChanged(item.id,Insert);
    return ret;
}


bool DbStates::modifyItem(const sStateItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":dev",item.dev);
    query.bindValue(":user",item.user);
    query.bindValue(":op",item.op);
    query.bindValue(":sn",item.sn);
    query.bindValue(":result",item.result);
    query.bindValue(":memo",item.memo);
    bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}
