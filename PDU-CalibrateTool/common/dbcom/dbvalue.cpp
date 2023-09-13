#include "dbvalue.h"

DbValue::DbValue()
{
    createTable();
    tableTile = tr("过程日志");
    //hiddens <<  9;
    headList << tr("设备类型") << tr("客户名称") << tr("设备序列号") << tr("状态") << tr("校准值");
}

void DbValue::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "dev            VCHAR,"
            "user           VCHAR,"
            "sn             VCHAR,"
            "result         VCHAR,"
            "value          VCHAR);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbValue *DbValue::bulid()
{
    static DbValue* sington = nullptr;
    if(sington == nullptr)
        sington = new DbValue();
    return sington;
}

bool DbValue::insertItem(const sValueItem &item)
{
    QString cmd = "insert into %1 (date,time,dev,user,sn,result,value) "
                  "values(:date,:time,:dev,:user,:sn,:result,:value)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    //if(ret) emit itemChanged(item.id,Insert);
    return ret;
}


bool DbValue::modifyItem(const sValueItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);

    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":dev",item.dev);
    query.bindValue(":user",item.user);
    query.bindValue(":sn",item.sn);
    query.bindValue(":result",item.result);
    query.bindValue(":value",item.value);
    bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}
