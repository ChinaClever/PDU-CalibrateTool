/*
 * db_usr.cpp
 * 用户数据库表
 *
 *  Created on: 2020年10月11日
 *      Author: Lzy
 */
#include "db_user.h"

DbUser::DbUser()
{
    createTable();
    tableTile = tr("用户管理");
    hiddens << 1 << 2 << 5 << 9;
    headList << tr("用户") << tr("权限") << tr("密码")
             << tr("邮箱") << tr("工号") << tr("备注")
             <<  tr("标记");
}

void DbUser::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           TEXT,"
            "time           TEXT,"
            "name           TEXT not null,"
            "jurisdiction   TEXT,"
            "pwd            TEXT,"
            "email          TEXT,"
            "telephone      VCHAR,"
            "remarks        VCHAR,"
            "jur            INTEGER);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))){
        throwError(query.lastError());
    }
}

DbUser *DbUser::bulid()
{
    static DbUser* sington = nullptr;
    if(sington == nullptr)
        sington = new DbUser();
    return sington;
}

bool DbUser::insertItem(sUserItem &item)
{
    item.id = maxId()+1;
    QString cmd = "insert into %1 (id,date,time,name,pwd,email,jurisdiction,telephone,remarks,jur) "
                  "values(:id,:date,:time,:name,:pwd,:email,:jurisdiction,:telephone,:remarks,:jur)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret) emit itemChanged(item.id,Insert);
    return ret;
}

int DbUser::contains(const QString &name)
{
    QString condition = QString("where name=\'%1\'").arg(name);

    return count("name", condition);
}

bool DbUser::updateItem(const sUserItem &item)
{
    QString cmd = "update %1 set "
                  "name        = :name,"
                  "pwd         = :pwd,"
                  "email       = :email,"
                  "jurisdiction   = :jurisdiction,"
                  "telephone   = :telephone,"
                  "remarks     = :remarks,"
                  "jur         = :jur"
                  " where id   = :id";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret) emit itemChanged(item.id,Update);

    return ret;
}

bool DbUser::modifyItem(const sUserItem &item, const QString &cmd)
{   
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":name",item.name);
    query.bindValue(":pwd",item.pwd);
    query.bindValue(":email",item.email);
    query.bindValue(":jurisdiction",item.jurisdiction);
    query.bindValue(":telephone",item.telephone);
    query.bindValue(":remarks",item.remarks);
    query.bindValue(":jur",item.jur);
    bool ret = query.exec();
    if(!ret)  throwError(query.lastError());
    return ret;
}

void DbUser::selectItem(QSqlQuery &query,sUserItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.name = query.value("name").toString();
    item.pwd = query.value("pwd").toString();
    item.email = query.value("email").toString();
    item.jurisdiction = query.value("jurisdiction").toString();
    item.telephone = query.value("telephone").toString();
    item.remarks = query.value("remarks").toString();
    item.jur = query.value("jur").toInt();
}


sUserItem DbUser::selItemsByName(const QString& Name)
{
    QVector<sUserItem> items = selectItems(QString("where name = \'%1\'").arg(Name));
    return items.first();
}


void DbUser::removeItemsByName(const QString& name)
{
    sUserItem item = selItemsByName(name);
    remove(item.id);
}
