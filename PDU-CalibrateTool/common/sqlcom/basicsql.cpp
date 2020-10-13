/*
 * BasicSql.cpp
 *
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "basicsql.h"

extern QString cm_pathOfData(const QString& name);

BasicSql::BasicSql(QObject *parent) :
    QObject(parent)
{
    mDb = initDb();
    headList << tr("编号") << tr("日期") << tr("时间");
    hiddens << 0;

    //标记表
    QString cmd = "create table if not exists markingtable("
                  "name TEXT primary key not null,"
                  "marking TEXT not null"
                  ");";
    QSqlQuery query(mDb);
    if(!query.exec(cmd))
        throwError(query.lastError());
}

BasicSql::~BasicSql()
{
    QSqlDatabase::database().commit();
    mDb.close();
}
/**
 * @brief 删除
 * @param condition
 */
bool BasicSql::remove(const QString &condition)
{
    QSqlQuery query(mDb);
    bool ret = query.exec(QString("DELETE FROM %1 WHERE %2").arg(tableName()).arg(condition));
    if(!ret) {
        throwError(query.lastError());
    }

    return ret;
}


bool BasicSql::remove(int id)
{
    bool ret =  remove(QString("id = %1").arg(id));
    if(ret) {
        emit itemChanged(id, Remove);
    }
    return ret;
}

bool BasicSql::removeMinIds(int id)
{
    bool ret =  remove(QString("id < %1").arg(id));
    if(ret) {
        emit itemChanged(id, Remove);
    }
    return ret;
}

bool BasicSql::removeMaxIds(int id)
{
    bool ret =  remove(QString("id > %1").arg(id));
    if(ret) {
        emit itemChanged(id, Remove);
    }
    return ret;
}

/**
 * @brief 获取最大ID
 * @param idName
 * @return
 */
int BasicSql::maxId()
{
    return maxId("id", "");
}


/**
 * @brief 获取最大ID
 * @param idName
 * @return
 */
int BasicSql::maxId(const QString &idName, const QString &condition)
{
    int id = 0;
    QSqlQuery query(mDb);
    if(query.exec(QString("select max(%1) from %2 %3").arg(idName).arg(tableName()).arg(condition)))
    {
        if(query.next())
            id = query.value(0).toInt();
    } else {
        throwError(query.lastError());
    }

    return id;
}

int BasicSql::maxId(const QString &condition)
{
    return maxId("id", QString("where %1").arg(condition));
}

int BasicSql::minId(const QString &idName, const QString &condition)
{
    int id = 0;
    QSqlQuery query(mDb);
    if(query.exec(QString("select MIN(%1) from %2 %3").arg(idName).arg(tableName()).arg(condition)))
    {
        if(query.next())
            id = query.value(0).toInt();
    } else {
        throwError(query.lastError());
    }

    return id;
}

int BasicSql::minId(const QString &condition)
{
    return minId("id", QString("where %1").arg(condition));
}

int BasicSql::minId()
{
    return minId("id", "");
}

bool BasicSql::minIdRemove()
{
    bool ret = false;
    int id = minId();
    if(id > 0) {
        ret = remove(id);
    }

    return ret;
}

bool BasicSql::minIdsRemove(int id)
{
    return removeMinIds(id);
}

/**
 * @brief 函数返回指定列的值的数目
 * @param column_name
 * @return
 */
int BasicSql::count(const QString &column_name, const QString &condition)
{
    int count = -1;
    QSqlQuery query(mDb);
    if(query.exec(QString("select count(DISTINCT %1) from %2 %3").arg(column_name).arg(tableName()).arg(condition))){
        if(query.next())
            count = query.value(0).toInt();
    } else
        throwError(query.lastError());
    return count;
}

int BasicSql::counts()
{
    return count("id", "");
}

/**
 * @brief 数据库最多保存多少条数据，删除掉其它的
 * @param count
 * @return
 */
bool BasicSql::countsRemove(int count)
{
    bool ret = false;
    int rtn = counts();
    if(rtn > count) {
        int id = minId() + (rtn-count);
        ret = minIdsRemove(id);
    }

    return ret;
}

/**
 * @brief 函数返回列的所有值
 * @param column_name 列名
 * @param condition 条件
 * @return
 */
QStringList BasicSql::listColumn(const QString &column_name, const QString &condition)
{
    QStringList list;
    QSqlQuery query(mDb);
    if(query.exec(QString("select DISTINCT %1 from %2 %3").arg(column_name).arg(tableName()).arg(condition))){
        while(query.next())
            list << query.value(0).toString();
    } else
        throwError(query.lastError());
    return list;
}


QVector<int> BasicSql::listColumnToInt(const QString &column_name, const QString &condition)
{
    QStringList list = listColumn(column_name, condition);
    QVector<int> items;
    for(int i=0; i<list.size(); ++i)
        items << list[i].toInt();

    return items;
}


bool BasicSql::updateColumn(const QString& column_name, double value, const QString &condition)
{
    bool ret = false;
    QSqlQuery query(mDb);
    ret = query.exec(QString("update  %1 set %2=%3 %4").arg(tableName()).arg(column_name).arg(value).arg(condition));
    if(!ret) throwError(query.lastError());
    return ret;
}

bool BasicSql::updateColumn(const QString& column_name, const QString& value, const QString &condition)
{
    bool ret = false;
    QSqlQuery query(mDb);
    ret = query.exec(QString("update  %1 set %2=\'%3\' where %4").arg(tableName()).arg(column_name).arg(value).arg(condition));
    if(!ret) throwError(query.lastError());
    return ret;
}

/**
 * @brief 函数返回id列的所有值
 * @param condition 查询条件
 * @return
 */
QVector<int> BasicSql::selectIds(const QString &condition)
{
    return listColumnToInt("id", condition);
}


/**
 * @brief 异常
 * @param err
 */
void BasicSql::throwError(const QSqlError &err)
{
    QString str;
    if(err.isValid()) //发生错误时isValid()返回true
    {
        switch (err.type()) {
        case QSqlError::NoError:
            str = tr("无错误");
            break;
        case QSqlError::ConnectionError://连接错语
            str = tr("连接错误");
            break;
        case QSqlError::StatementError://语句错语
            str = tr("语句错误");
            break;
        case QSqlError::TransactionError://事务错误
            str = tr("事务错误");
            break;
        case QSqlError::UnknownError: //未知错误
            str = tr("未知错误");
            break;
        }
    }

    qCritical() << "Sql_Error: " << tr("数据库名：") << tableName() << str << err.text();
}

bool BasicSql::clear()
{
    return remove("id > 0");
}

/**
 * @brief BasicSql::tableMarking
 * @return
 */
QString BasicSql::tableMarking()
{
    QString cmd = QString("SELECT * from markingtable where name = \'%1\'").arg(tableName());
    QSqlQuery query(mDb);
    if(query.exec(cmd)){
        if(query.next())
            return query.value(1).toString();
    } else {
        throwError(query.lastError());
    }
    return "";
}

/**
 * @brief BasicSql::setTableMarking
 * @param marking
 */
void BasicSql::setTableMarking(const QString &marking)
{
    QString ori = tableMarking();
    QString cmd = ori.isEmpty()?"insert into markingtable (name,marking) values(%1,%2)":"update markingtable set marking = %2 where name = \"%1\"";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()).arg(marking)))
        throwError(query.lastError());
}

/**
 * @brief 数据库初始化
 */
QSqlDatabase BasicSql::initDb()
{
#if 1
    static QSqlDatabase db;
    if(!db.isOpen()){
        db = QSqlDatabase::addDatabase("QSQLITE");
        CfgCom *cfg = CfgCom::bulid();
        db.setDatabaseName(cfg->pathOfData("cali_log.db"));
        if (!db.open()) { //打开数据库
            qDebug() << "init Db error !!!";
        }
    }
#else
    QSqlDatabase db;
    quint32 value = QRandomGenerator::global()->generate();
    if (QSqlDatabase::contains(QString::number(value))) {
        db = QSqlDatabase::database(QString::number(value));
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", QString::number(value));
        db.setDatabaseName(cm_pathOfData("cali_log.db"));
        if (!db.open()) { //打开数据库
            qDebug() << "init Db error !!!" << db.lastError().text();
        }
    }
#endif

    return db;
}


/**
 * @brief 根据日期来查询
 */
void DB_selectTableByTime(QString &name, QString &start, QString &end, QList<QStringList> &list)
{
    QSqlDatabase db = BasicSql::initDb();
    QSqlQuery query(db);
    QString cmd = QString("select * from %1 where date Between \'%2\' and  \'%3\'").arg(name).arg(start).arg(end);
    bool ret = query.exec(cmd);
    if(ret)
    {
        while(query.next())
        {
            QStringList strList;
            int columnNum = query.record().count(); //获取每条记录中属性（即列）的个数
            for(int i=0; i<columnNum; ++i)
                strList << query.value(i).toString();
            list.append(strList);
        }
    } else
        qDebug() << cmd << "Err";
}


/**
 * @brief 获取日志时间链表
 * @return
 */
bool db_select_dateList(const QString &tableName, QStringList &dateList)
{
    QString str = QString("select * from %1").arg(tableName);

    QSqlDatabase db = BasicSql::initDb();
    QSqlQuery query(db);
    query.setForwardOnly(true); // 为了节省内存开销
    bool ret = query.exec(str);
    if(ret) {
        while(query.next()) //开始就先执行一次next()函数，那么query 指向结果集的第一条记录
        {
            str = query.value(1).toString();  //日期
            if(!dateList.contains(str))
                dateList << str; // 加入链表中
        }
    } else
        qDebug() << "db_select_dateList err: " << str;

    return ret;
}
