/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_build.h"


Json_Build::Json_Build()
{
    QString product[PRODUCT_NUM]={"ZPDU","MPDU","RPDU","NPM-V","IP-PDU","SI-PDU","BM-PDU"};
    for(int i = 0 ; i < PRODUCT_NUM ; i++)
    {
        mProduct.push_back(product[i]);//产品种类初始化
    }
}

Json_Build *Json_Build::bulid()
{
    static Json_Build* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Build();
    }
    return sington;
}


void Json_Build::head(QJsonObject &obj)
{
    obj.insert("company", "clever");
    obj.insert("version", SN_TOTAL_NUM);

    pduInfo(obj);
}

void Json_Build::pduInfo(QJsonObject &obj)
{
    QVector<QString> devtype;
    createDevType(devtype);
    objData(obj,devtype);
}

void Json_Build::createDevType( QVector<QString>& devtype)
{
    QString str[SN_TOTAL_NUM] =
    {
        "ZPDU_交流_锰铜_A系列_单相_4位",//0x0000 0001
        "ZPDU_交流_锰铜_B系列_单相_4位",//0x0000 0002
        "ZPDU_交流_锰铜_C系列_单相_4位",//0x0000 0003
        "ZPDU_交流_锰铜_D系列_单相_4位",//0x0000 0004

        "ZPDU_交流_锰铜_A系列_单相_8位",//0x0000 0005
        "ZPDU_交流_锰铜_B系列_单相_8位",//0x0000 0006
        "ZPDU_交流_锰铜_C系列_单相_8位",//0x0000 0007
        "ZPDU_交流_锰铜_D系列_单相_8位",//0x0000 0008

        "ZPDU_交流_锰铜_A系列_单相_12位",//0x0000 0009
        "ZPDU_交流_锰铜_B系列_单相_12位",//0x0000 000A
        "ZPDU_交流_锰铜_C系列_单相_12位",//0x0000 000B
        "ZPDU_交流_锰铜_D系列_单相_12位",//0x0000 000C

        "ZPDU_交流_锰铜_A系列_单相_14位",//0x0000 000D
        "ZPDU_交流_锰铜_B系列_单相_14位",//0x0000 000E
        "ZPDU_交流_锰铜_C系列_单相_14位",//0x0000 000F
        "ZPDU_交流_锰铜_D系列_单相_14位",//0x0000 0010

        "ZPDU_交流_锰铜_A系列_三相_4位",//0x0000 0011
        "ZPDU_交流_锰铜_B系列_三相_4位",//0x0000 0012
        "ZPDU_交流_锰铜_C系列_三相_4位",//0x0000 0013
        "ZPDU_交流_锰铜_D系列_三相_4位",//0x0000 0014

        "ZPDU_交流_锰铜_A系列_三相_8位",//0x0000 0015
        "ZPDU_交流_锰铜_B系列_三相_8位",//0x0000 0016
        "ZPDU_交流_锰铜_C系列_三相_8位",//0x0000 0017
        "ZPDU_交流_锰铜_D系列_三相_8位",//0x0000 0018

        "ZPDU_交流_锰铜_A系列_三相_12位",//0x0000 0019
        "ZPDU_交流_锰铜_B系列_三相_12位",//0x0000 001A
        "ZPDU_交流_锰铜_C系列_三相_12位",//0x0000 001B
        "ZPDU_交流_锰铜_D系列_三相_12位",//0x0000 001C

        "ZPDU_交流_锰铜_A系列_三相_14位",//0x0000 001D
        "ZPDU_交流_锰铜_B系列_三相_14位",//0x0000 001E
        "ZPDU_交流_锰铜_C系列_三相_14位",//0x0000 001F
        "ZPDU_交流_锰铜_D系列_三相_14位",//0x0000 0020

        "ZPDU_交流_互感器_A系列_单相_4位",//0x0000 0021
        "ZPDU_交流_互感器_B系列_单相_4位",//0x0000 0022
        "ZPDU_交流_互感器_C系列_单相_4位",//0x0000 0023
        "ZPDU_交流_互感器_D系列_单相_4位",//0x0000 0024

        "ZPDU_交流_互感器_A系列_单相_8位",//0x0000 0025
        "ZPDU_交流_互感器_B系列_单相_8位",//0x0000 0026
        "ZPDU_交流_互感器_C系列_单相_8位",//0x0000 0027
        "ZPDU_交流_互感器_D系列_单相_8位",//0x0000 0028

        "ZPDU_交流_互感器_A系列_单相_12位",//0x0000 0029
        "ZPDU_交流_互感器_B系列_单相_12位",//0x0000 002A
        "ZPDU_交流_互感器_C系列_单相_12位",//0x0000 002B
        "ZPDU_交流_互感器_D系列_单相_12位",//0x0000 002C

        "ZPDU_交流_互感器_A系列_单相_14位",//0x0000 002D
        "ZPDU_交流_互感器_B系列_单相_14位",//0x0000 002E
        "ZPDU_交流_互感器_C系列_单相_14位",//0x0000 002F
        "ZPDU_交流_互感器_D系列_单相_14位",//0x0000 0030

        "ZPDU_交流_互感器_A系列_三相_4位",//0x0000 0031
        "ZPDU_交流_互感器_B系列_三相_4位",//0x0000 0032
        "ZPDU_交流_互感器_C系列_三相_4位",//0x0000 0033
        "ZPDU_交流_互感器_D系列_三相_4位",//0x0000 0034

        "ZPDU_交流_互感器_A系列_三相_8位",//0x0000 0035
        "ZPDU_交流_互感器_B系列_三相_8位",//0x0000 0036
        "ZPDU_交流_互感器_C系列_三相_8位",//0x0000 0037
        "ZPDU_交流_互感器_D系列_三相_8位",//0x0000 0038

        "ZPDU_交流_互感器_A系列_三相_12位",//0x0000 0039
        "ZPDU_交流_互感器_B系列_三相_12位",//0x0000 003A
        "ZPDU_交流_互感器_C系列_三相_12位",//0x0000 003B
        "ZPDU_交流_互感器_D系列_三相_12位",//0x0000 003C

        "ZPDU_交流_互感器_A系列_三相_14位",//0x0000 003D
        "ZPDU_交流_互感器_B系列_三相_14位",//0x0000 003E
        "ZPDU_交流_互感器_C系列_三相_14位",//0x0000 003F
        "ZPDU_交流_互感器_D系列_三相_14位",//0x0000 0040

        "ZPDU_磁保持_锰铜_C系列_单相_4位",//0x0000 0041
        "ZPDU_磁保持_锰铜_D系列_单相_4位",//0x0000 0042

        "ZPDU_磁保持_锰铜_C系列_单相_8位",//0x0000 0043
        "ZPDU_磁保持_锰铜_D系列_单相_8位",//0x0000 0044

        "ZPDU_磁保持_锰铜_C系列_单相_12位",//0x0000 0045
        "ZPDU_磁保持_锰铜_D系列_单相_12位",//0x0000 0046

        "ZPDU_磁保持_锰铜_C系列_单相_14位",//0x0000 0047
        "ZPDU_磁保持_锰铜_D系列_单相_14位",//0x0000 0048

        "ZPDU_磁保持_锰铜_C系列_三相_4位",//0x0000 0049
        "ZPDU_磁保持_锰铜_D系列_三相_4位",//0x0000 004A

        "ZPDU_磁保持_锰铜_C系列_三相_8位",//0x0000 004B
        "ZPDU_磁保持_锰铜_D系列_三相_8位",//0x0000 004C

        "ZPDU_磁保持_锰铜_C系列_三相_12位",//0x0000 004D
        "ZPDU_磁保持_锰铜_D系列_三相_12位",//0x0000 004E

        "ZPDU_磁保持_锰铜_C系列_三相_14位",//0x0000 004F
        "ZPDU_磁保持_锰铜_D系列_三相_14位",//0x0000 0050

        "ZPDU_磁保持_互感器_C系列_单相_4位",//0x0000 0051
        "ZPDU_磁保持_互感器_D系列_单相_4位",//0x0000 0052

        "ZPDU_磁保持_互感器_C系列_单相_8位",//0x0000 0053
        "ZPDU_磁保持_互感器_D系列_单相_8位",//0x0000 0054

        "ZPDU_磁保持_互感器_C系列_单相_12位",//0x0000 0055
        "ZPDU_磁保持_互感器_D系列_单相_12位",//0x0000 0056

        "ZPDU_磁保持_互感器_C系列_单相_14位",//0x0000 0057
        "ZPDU_磁保持_互感器_D系列_单相_14位",//0x0000 0058

        "ZPDU_磁保持_互感器_C系列_三相_4位",//0x0000 0059
        "ZPDU_磁保持_互感器_D系列_三相_4位",//0x0000 005A

        "ZPDU_磁保持_互感器_C系列_三相_8位",//0x0000 005B
        "ZPDU_磁保持_互感器_D系列_三相_8位",//0x0000 005C

        "ZPDU_磁保持_互感器_C系列_三相_12位",//0x0000 005D
        "ZPDU_磁保持_互感器_D系列_三相_12位",//0x0000 005E

        "ZPDU_磁保持_互感器_C系列_三相_14位",//0x0000 005F
        "ZPDU_磁保持_互感器_D系列_三相_14位",//0x0000 0060
    };

    int len = SN_TOTAL_NUM;
    for(int i = 0 ; i < len ; i++)
    {
        devtype.push_back(str[i]);
    }
}

int Json_Build::objData(QJsonObject &obj,QVector<QString>& devtype)
{
    QJsonArray jsonArray;
    int num = SN_TOTAL_NUM;
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("id",i+1);
        subObj.insert("SN_NO", QString("0x%1").arg(i+1,8,16,QLatin1Char('0')));
        subObj.insert("dev_type", devtype[i]);

        jsonArray.append(subObj);
    }
    obj.insert("ZPDU" ,QJsonValue(jsonArray));

    return num;
}

void Json_Build::getJson(QJsonObject &json)
{
    head(json);
    saveJson(json);
}

bool Json_Build::saveJson( QJsonObject &json)
{
    QJsonDocument jsonDoc(json);
    QByteArray ba = jsonDoc.toJson();
    QString path = CfgCom::bulid()->pathOfData("pdu_id.json");
    QFile file(path);
    bool ret = false;
    if(file.exists())//文件存在则不需要再写
    {
        ret = true;
    }
    else//文件不存在则写入初始数据
    {
        ret = file.open(QIODevice::WriteOnly);
        if(ret) {
            file.write(ba);
            file.close();
        } else {
            qDebug() << "write json file failed";
        }
    }

    return ret;
}

