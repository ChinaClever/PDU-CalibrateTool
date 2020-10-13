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

    initCnt();
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
    QString value = QDate::currentDate().toString("yyyy-MM-dd");
    write("date", value, "Date");
}

bool Ad_Config::getDate()
{
    bool ret = false;

    QString str = read("date","","Date").toString();
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
    write("num", item->currentNum, "Date");
}

void Ad_Config::initCurrentNum()
{
    bool ret = getDate();
    if(ret) {
        item->currentNum = 0;
        setCurrentNum();
    } else {
        int value = read("num", 0,"Date").toInt();
        item->currentNum = value;
    }
}

int Ad_Config::getAddr()
{
    return read("addr", 3,"Sys").toInt();
}

void Ad_Config::setAddr(int addr)
{
    write("addr", addr, "Sys");
}

void Ad_Config::initErrData()
{
    item->volErr = read("vol", 1,"Err").toInt();
    item->curErr = read("cur", 1,"Err").toInt();
    item->powErr = read("pow", 15,"Err").toInt();
}

void Ad_Config::writeErrData()
{
    write("vol", item->volErr, "Err");
    write("cur", item->curErr, "Err");
    write("pow", item->powErr, "Err");
}

void Ad_Config::initCnt()
{
    item->cnt.all = read("all", 0, "Count").toInt();
    item->cnt.ok = read("ok", 0, "Count").toInt();
    item->cnt.err = read("err", 0, "Count").toInt();
}

void Ad_Config::writeCnt()
{
    write("all", item->cnt.all, "Count");
    write("ok", item->cnt.ok, "Count");
    write("err", item->cnt.err, "Count");
}

void Ad_Config::initThreshold()
{
    item->user = read("user", "").toString();
    item->cTh.repair_en = read("repair_en", 0).toInt();
    item->cTh.type = read("cth_type", 1).toInt();
    item->cTh.vol_min = read("vol_min", 80).toInt();
    item->cTh.vol_max = read("vol_max", 276).toInt();
    item->cTh.cur_min = read("cur_min", 0).toInt();
    item->cTh.cur_max = read("cur_max", 320).toInt();
    item->cTh.si_mod = read("si_mod", 0).toInt();

    item->cTh.ip_version = read("ip_version", 1).toInt();
    item->cTh.ip_lines = read("ip_lines", 1).toInt();
    item->cTh.ip_mod = read("ip_modbus", 0).toInt();
    item->cTh.ip_log = read("log_en", 0).toInt();
    item->cTh.ip_standard = read("ip_standard", 0).toInt();
    item->cTh.ip_language = read("ip_language", 1).toInt();

    char* ch = getMacAddr().toLatin1().data();
    strcpy(item->cTh.mac_addr, ch);
}

void Ad_Config::writeThreshold()
{
    write("user", item->user);
    write("repair_en", item->cTh.repair_en);
    write("cth_type", item->cTh.type);
    write("vol_min", item->cTh.vol_min);
    write("vol_max", item->cTh.vol_max);
    write("cur_min", item->cTh.cur_min);
    write("cur_max", item->cTh.cur_max);
    write("si_mod", item->cTh.si_mod);

    write("ip_version", item->cTh.ip_version);
    write("ip_lines", item->cTh.ip_lines);
    write("ip_modbus", item->cTh.ip_mod);
    write("log_en", item->cTh.ip_log);
    write("ip_standard", item->cTh.ip_standard);
    write("ip_language", item->cTh.ip_language);

    setMacAddr(item->cTh.mac_addr);
}


/**
 * @brief 获取当前用户名称
 * @return 用户名
 */
QString Ad_Config::getMacAddr()
{
    return read("mac", "").toString();
}

/**
 * @brief 设置当前用户名
 * @param name
 */
void Ad_Config::setMacAddr(const QString &name)
{
    write("mac", name);
}
