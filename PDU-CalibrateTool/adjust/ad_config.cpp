/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_config.h"

Ad_Config::Ad_Config()
{
    item = new sConfigItem();
    item->serial = nullptr;
    item->source = nullptr;
    item->addr = getAddr();

    initErrData();
    initThreshold();
    initCurrentNum();
}

Ad_Config *Ad_Config::bulid()
{
    static Ad_Config* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Config();
    return sington;
}


void Ad_Config::setDate()
{
    QString prefix = getPrefix();
    QString str = QString("%1_date_current").arg(prefix);
    QString value = QDate::currentDate().toString("yyyy-MM-dd");
    com_cfg_write(str, value, prefix);
}

bool Ad_Config::getDate()
{
    bool ret = false;
    QString prefix = getPrefix();
    QString str = QString("%1_date_current").arg(prefix);
    str = com_cfg_readStr(str, prefix);
    if(!str.isEmpty()) {
        QDate date = QDate::fromString(str, "yyyy-MM-dd");
        if(QDate::currentDate() > date) {
            ret = true;
        }
    }

    return ret;
}

void Ad_Config::setCurrentNum()
{
    setDate();
    setValue(item->currentNum, "current","num");
}

void Ad_Config::initCurrentNum()
{
    bool ret = getDate();
    if(ret) {
        item->currentNum = 0;
        setCurrentNum();
    } else {
        int value = getValue("current","num");
        if(value < 1) value = 0;
        item->currentNum = value;
    }
}

QString Ad_Config::initName()
{
    QString name= getCurrentName();
    if(name.isEmpty()) {
        item->currentName = "admin";
        setCurrentName(item->currentName);
    } else {
        item->currentName = name;
    }
    return item->currentName;
}

void Ad_Config::setName(QString str)
{
    setCurrentName(str);
}

int Ad_Config::getAddr()
{
    int addr = getValue("addr", "id");
    if(addr < 0) addr = 3;

    return addr;
}

void Ad_Config::setAddr(int addr)
{
    setValue(addr, "addr", "id");
}


void Ad_Config::initErrData()
{
    double value = getValue("err", "vol");
    if(value < 0) value = 1;
    item->volErr = value;

    value = getValue("err", "cur");
    if(value < 0) value = 1;
    item->curErr = value;

    value = getValue("err", "pow");
    if(value < 0) value = 15;
    item->powErr = value;
}

void Ad_Config::writeErrData()
{
    double value = item->volErr;
    setValue(value, "err", "vol");

    value = item->curErr;
    setValue(value, "err", "cur");

    value = item->powErr;
    setValue(value, "err", "pow");
}

void Ad_Config::initThreshold()
{
    double value = getValue("cth", "type");
    if(value < 0) value = 1;
    item->cTh.type = value;

    value = getValue("vol", "min");
    if(value < 0) value = 80;
    item->cTh.vol_min = value;

    value = getValue("vol", "max");
    if(value < 0) value = 276;
    item->cTh.vol_max = value;

    value = getValue("cur", "min");
    if(value < 0) value = 0;
    item->cTh.cur_min = value;

    value = getValue("cur", "max");
    if(value < 0) value = 320;
    item->cTh.cur_max = value;

    value = getValue("si", "mod");
    if(value < 0) value = 0;
    item->cTh.si_mod = value;

    value = getValue("ele", "clear");
    if(value < 0) value = 1;
    item->cTh.ele_clear = value;

    value = getValue("ip", "mod");
    if(value < 0) value = 0;
    item->cTh.ip_mod = value;

    value = getValue("mac", "set");
    if(value < 0) value = 1;
    item->cTh.mac_set = value;

    value = getValue("mac", "clear");
    if(value < 0) value = 0;
    item->cTh.mac_clear = value;

    value = getValue("time", "set");
    if(value < 0) value = 1;
    item->cTh.time_set = value;

    value = getValue("log", "clear");
    if(value < 0) value = 1;
    item->cTh.log_clear = value;

    char* ch = getMacAddr().toLatin1().data();
    strcpy(item->cTh.mac_addr, ch);
}


void Ad_Config::writeThreshold()
{
    double value = item->cTh.type;
    setValue(value, "cth", "type");

    value = item->cTh.vol_min;
    setValue(value, "vol", "min");

    value = item->cTh.vol_max;
    setValue(value, "vol", "max");

    value = item->cTh.cur_min;
    setValue(value, "cur", "min");

    value = item->cTh.cur_max;
    setValue(value, "cur", "max");

    value = item->cTh.si_mod;
    setValue(value, "si", "mod");

    value = item->cTh.ele_clear;
    setValue(value, "ele", "clear");

    value = item->cTh.ip_mod;
    setValue(value, "ip", "mod");

    value = item->cTh.mac_set;
    setValue(value, "mac", "set");

    value = item->cTh.mac_clear;
    setValue(value, "mac", "clear");

    value = item->cTh.time_set;
    setValue(value, "time", "set");

    value = item->cTh.log_clear;
    setValue(value, "log", "clear");

    setMacAddr(item->cTh.mac_addr);
}


/**
 * @brief 获取当前用户名称
 * @return 用户名
 */
QString Ad_Config::getMacAddr()
{
    QString prefix = getPrefix();
    QString str = QString("%1_mac_addr").arg(prefix);
    return com_cfg_readStr(str, prefix);
}

/**
 * @brief 设置当前用户名
 * @param name
 */
void Ad_Config::setMacAddr(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_mac_addr").arg(prefix);
    com_cfg_write(str, name, prefix);
}
