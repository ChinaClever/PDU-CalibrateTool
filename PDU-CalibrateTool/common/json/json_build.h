#ifndef BUILDJSON_H
#define BUILDJSON_H
#include "datapacket.h"
#include "configbase.h"

#define SN_TOTAL_NUM 96
#define PRODUCT_NUM 7

class Json_Build
{
    Json_Build();
public:
    static Json_Build *bulid();
    void getJson(QJsonObject &json);
    QVector<QString> mProduct;

protected:
    void head(QJsonObject &obj);
    void pduInfo(QJsonObject &json);
    int objData( QJsonObject &obj,QVector<QString>& devtype);
    void createDevType( QVector<QString>& devtype);
    bool saveJson(QJsonObject &json);
};

#endif // BUILDJSON_H
