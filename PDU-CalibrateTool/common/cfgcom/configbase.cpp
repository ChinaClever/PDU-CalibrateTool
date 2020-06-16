/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{

}

ConfigBase *ConfigBase::bulid()
{
    static ConfigBase* sington = nullptr;
    if(sington == nullptr)
        sington = new ConfigBase();
    return sington;
}


/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString ConfigBase::getSerialName(const QString &com)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM_%2").arg(prefix).arg(com);
    return com_cfg_readStr(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void ConfigBase::setSerialName(const QString &name, const QString &com)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM_%2").arg(prefix).arg(com);
    com_cfg_write(str, name, prefix);
}


/**
 * @brief 获取串口名称
 * @return 串口名
 */
double ConfigBase::getValue(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_value_%2").arg(prefix).arg(name);
    return com_cfg_readDouble(str, prefix);
}

/**
 * @brief 设置串口名
 * @param name
 */
void ConfigBase::setValue(double value, const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_COM_%2").arg(prefix).arg(name);
    com_cfg_write(str, QString::number(value), prefix);
}
