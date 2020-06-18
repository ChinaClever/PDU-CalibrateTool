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
    setValue(item->currentNum, "current_num");
}

void Ad_Config::initCurrentNum()
{
    bool ret = getDate();
    if(ret) {
        item->currentNum = 0;
        setCurrentNum();
    } else {
        int value = getValue("current_num");
        if(value < 1) value = 0;
        item->currentNum = value;
    }
}
