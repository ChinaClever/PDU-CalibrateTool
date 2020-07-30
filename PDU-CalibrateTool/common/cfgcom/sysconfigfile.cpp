/*
 * sysconfigfile.cpp
 *  配置文件的操作接口
 *  配置参数的保存与读取
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "sysconfigfile.h"



static QSettings *pConfigIni = NULL;

/***
  * 获取程序数据目录
  */
QString cm_pathOfData(const QString& name)
{
    QDir dataDir(QDir::home());
    QString dirName = ".PDU-CalibrateTool";
    if(!dataDir.exists(dirName))
        dataDir.mkdir(dirName);
    dataDir.cd(dirName);
    return dataDir.absoluteFilePath(name);
}

/**
 * 功 能：打开系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
bool com_cfg_open(void)
{
    QString strFilename = cm_pathOfData( "sysconfig.ini");
    bool ret = QFileInfo(strFilename).exists();
    if(pConfigIni==NULL) {
        pConfigIni = new QSettings(strFilename, QSettings::IniFormat);
        pConfigIni->setIniCodec(QTextCodec::codecForName("utf-8")); // gb18030
    }

    return ret;
}

/**
 * 功 能：关闭系统配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void com_cfg_close(void)
{
    // delete pConfigIni;
    // pConfigIni = NULL;

    pConfigIni->sync();
}

/**
 * 功 能：读字符串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
QString com_cfg_readStr(const QString& str, const QString& strGroup)
{
    com_cfg_open();
    QString strParameterName = "/" + strGroup + "/" + str;
    QString strParameter = pConfigIni->value(strParameterName).toString();
    com_cfg_close();

    return strParameter;
}

/**
 * 功 能：读整形串配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
int com_cfg_readInt(const QString& strParameterName, const QString& strGroup)
{       
    bool ok;

    int ret = com_cfg_readStr(strParameterName, strGroup).toInt(&ok);
    if(!ok)  ret = -1;

    return ret;
}

/**
 * 功 能：读浮点形串配置文件
 * 开发人员：Lzy     2013 - 七夕
 */
double com_cfg_readDouble(const QString& strParameterName, const QString& strGroup)
{
    bool ok;

    double ret = com_cfg_readStr(strParameterName, strGroup).toDouble(&ok);
    if(!ok)  ret = -1;

    return ret;
}

/**
 * 功 能：参数写入配置文件
 * 开发人员：Lzy     2016 - 七夕
 */
void com_cfg_write(const QString& str, const QString& strParameter, const QString& strGroup)
{
    QString strParameterName = "/" + strGroup + "/" + str;
    pConfigIni->setValue(strParameterName, strParameter);
}

/**
 * 功 能：写入参数
 * 开发人员：Lzy     2016 - 七夕
 */
void com_cfg_writeParam(const QString&  name, const QString&  value, const QString&  strGroup)
{
    com_cfg_open();
    com_cfg_write(name, value, strGroup);
    com_cfg_close();
}
