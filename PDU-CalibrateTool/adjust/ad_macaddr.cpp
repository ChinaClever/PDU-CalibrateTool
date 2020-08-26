/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_macaddr.h"

#define MAC_ADDR_LEN 6

Ad_MacAddr::Ad_MacAddr()
{

}

Ad_MacAddr *Ad_MacAddr::bulid()
{
    static Ad_MacAddr* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_MacAddr();
    return sington;
}

bool Ad_MacAddr::isMacAddress(const QString &mac)
{
    QString mac_addr = mac;
    QRegExp rx("^([A-Fa-f0-9]{2}[-,:]){5}[A-Fa-f0-9]{2}$");
    QRegExpValidator v(rx, 0);
    int pos = 0;
    if(v.validate(mac_addr, pos) == QValidator::Acceptable)
        return true;

    return false;
}


/**
* 函数功能:将格式如"EA-EB-EC-AA-AB-AC"的MAC QString字符串转换成6字节的数组，
* 数组的元素对应为{0xEA,0xEB,0xEC,0xAA,0xAB,0xAC}
*/
QByteArray Ad_MacAddr::get_mac_array_from_QString(const QString &in)
{
    QByteArray array;
    unsigned char out[6];
    uint8_t next_ip_separate_symbol_index = 0;
    uint8_t now_ip_separate_symbol_index = 0;

    for(uint8_t i=0;i<6;i++){
        next_ip_separate_symbol_index = (i!=5)? (in.indexOf("-",next_ip_separate_symbol_index+1)):(in.length());
        if((next_ip_separate_symbol_index-now_ip_separate_symbol_index>0)&&(next_ip_separate_symbol_index-now_ip_separate_symbol_index<= 3)){
            *(out+i) = in.mid((i==0)? now_ip_separate_symbol_index:(now_ip_separate_symbol_index+1), \
                              (i==0)? next_ip_separate_symbol_index: \
                                      (next_ip_separate_symbol_index-now_ip_separate_symbol_index-1)).toInt(0,16);
            now_ip_separate_symbol_index = next_ip_separate_symbol_index;
        }else{
            return array;
        }
    }

    array.resize(sizeof(out));//重置数据大小
    memcpy(array.data(), out, sizeof(out));//copy数据

    return array;
}

quint64 Ad_MacAddr::fromBigEndian(const QByteArray &bytes)
{
    union { uchar a[8]; quint64 b; }d;

    d.b = 0;
    for(int i=0; i<MAC_ADDR_LEN; ++i) {
        d.a[i] = bytes.at(bytes.size()-i-1);
    }

    return d.b;
}

QByteArray Ad_MacAddr::toLittleEndian(quint64 v)
{
    union { uchar a[8]; quint64 b; }d;

    d.b = v;
    QByteArray bytes;
    for(int i=0; i<MAC_ADDR_LEN; ++i) {
        bytes.insert(0, d.a[i]);
    }

    return bytes;
}

quint64 Ad_MacAddr::macToInt(const QString &in)
{
    quint64 ret = 0;
    bool ok = isMacAddress(in);
    if(ok) {
        QByteArray bytes = get_mac_array_from_QString(in);
        ret = fromBigEndian(bytes);
    } else {
        qDebug() << "Mac Addr Err" << in;
    }

    return ret;
}

QString Ad_MacAddr::intToMac(quint64 v)
{
    QByteArray array = toLittleEndian(v);
    QString str = cm_ByteArrayToHexStr(array);
    QString mac = str.left(str.size()-1);
    return mac.replace(QString(" "), QString("-"));
}

int Ad_MacAddr::intToChar(quint64 v, uchar *buf)
{
    QByteArray array = toLittleEndian(v);
    for(int i=0; i<array.size(); ++i) {
        buf[i] = array.at(i);
    }

    return array.size();
}

int Ad_MacAddr::macToChar(const QString &in, uchar *buf)
{
    int ret = 0;
    quint64 mac = macToInt(in);
    if(mac > 0) {
        ret = intToChar(mac, buf);
    }

    return ret;
}

QString Ad_MacAddr::macAdd(const QString &in, char *buf)
{
    QString ret;
    quint64 mac = macToInt(in);
    if(mac++ > 0) {
        ret = intToMac(mac);
        strcpy(buf, ret.toLatin1().data());
    }

    return ret;
}
