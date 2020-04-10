#ifndef COMMON_H
#define COMMON_H
#include "msgbox.h"
#include "configbase.h"

QString cm_pathOfData(const QString& name);
bool cm_isDigitStr(const QString &src);
bool cm_isIPaddress(const QString& ip);

QString cm_ByteArrayToHexStr(const QByteArray &array);
QString cm_ByteArrayToUcharStr(const QByteArray &array);


#endif // COMMON_H
