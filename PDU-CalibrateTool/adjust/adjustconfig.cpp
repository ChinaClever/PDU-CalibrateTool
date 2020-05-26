/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "adjustconfig.h"

AdjustConfig::AdjustConfig()
{
    item = new sConfigItem();
    item->serial = nullptr;
    item->source = nullptr;
}

AdjustConfig *AdjustConfig::bulid()
{
    static AdjustConfig* sington = nullptr;
    if(sington == nullptr)
        sington = new AdjustConfig();
    return sington;
}

/**
 * @brief 获取串口名称
 * @return 串口名
 */
double AdjustConfig::getValue(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_value_%2").arg(prefix).arg(name);
    return com_cfg_readDouble(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void AdjustConfig::setValue(double value, const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM_%2").arg(prefix).arg(name);
    com_cfg_write(str, QString::number(value), prefix);
}
