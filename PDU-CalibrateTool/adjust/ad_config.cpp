/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "ad_config.h"

Ad_Config::Ad_Config()
{
    item = new sConfigItem();
    item->serial = nullptr;
    item->source = nullptr;
}

Ad_Config *Ad_Config::bulid()
{
    static Ad_Config* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Config();
    return sington;
}

/**
 * @brief 获取串口名称
 * @return 串口名
 */
double Ad_Config::getValue(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_value_%2").arg(prefix).arg(name);
    return com_cfg_readDouble(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void Ad_Config::setValue(double value, const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM_%2").arg(prefix).arg(name);
    com_cfg_write(str, QString::number(value), prefix);
}
