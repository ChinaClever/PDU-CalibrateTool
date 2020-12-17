/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_config.h"

Ad_Config::Ad_Config()
{
    item = new sConfigItem();
    item->jig = nullptr;
    item->serial = nullptr;
    item->source = nullptr;
    item->addr = getAddr();

    item->si_led = false;
    item->si_ac = 1;
    item->si_line = 1;

    initCnt();
    initErrData();
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
    item->cnt.num = read("num", 0, "Cnt").toInt();
    item->cnt.all = read("all", 0, "Cnt").toInt();
    item->cnt.ok = read("ok", 0, "Cnt").toInt();
    item->cnt.err = read("err", 0, "Cnt").toInt();
    item->user = read("user", "", "User").toString();
}

void Ad_Config::writeCnt()
{
    write("num", item->cnt.num, "Cnt");
    write("all", item->cnt.all, "Cnt");
    write("ok", item->cnt.ok, "Cnt");
    write("err", item->cnt.err, "Cnt");
    write("user", item->user, "User");
}

