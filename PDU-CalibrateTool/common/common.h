#ifndef COMMON_H
#define COMMON_H
#include "msgbox.h"
#include "configbase.h"

bool cm_checkIp(const QString& ip);
bool cm_isDigitStr(const QString &src);
bool cm_isIPaddress(const QString& ip);
void print(const QString& array);

QString cm_ByteArrayToHexStr(const QByteArray &array);
QString cm_ByteArrayToUcharStr(const QByteArray &array);


#endif // COMMON_H
