/*
 * common.cpp
 * 公共方法接口
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "common.h"
#include <QWidget>
#include <QHeaderView>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

bool cm_pingNet(const QString& ip)
{
    QProcess pingProcess;
    QString strArg = "ping " + ip + " -n 1 -i 2";  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit(pingProcess.readAllStandardOutput());
    bool bPingSuccess = false;

    if(p_stdout.contains("TTL=")) { //我采用这个字符来判断 对不对？
        bPingSuccess = true;
    }else {
        bPingSuccess = false;
    }

    return bPingSuccess;
}

/***
  *判断一个字符串是否为纯数字
  */
bool cm_isDigitStr(const QString &src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s) //不是纯数字
        return false;

    return true;
}

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool cm_isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}


/**
 * @brief 转16进制
 * @param array
 * @return
 */
QString cm_ByteArrayToHexStr(const QByteArray &array)
{
    QString strArray = array.toHex().toUpper(); // 十六进制
    for(int i=0; i<array.size(); ++i)
        strArray.insert(2+3*i, " "); // 插入空格
    return strArray;
}

/**
 * @brief 转十进制
 * @param array
 * @return
 */
QString cm_ByteArrayToUcharStr(const QByteArray &array)
{
    QString strArray ;
    for(int i=0; i<array.size(); ++i)
        strArray += QString::number((uchar)array.at(i)) + " ";  // 十进制
    return strArray;
}


