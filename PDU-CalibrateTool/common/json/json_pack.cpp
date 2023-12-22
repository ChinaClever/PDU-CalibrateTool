/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_pack.h"


Json_Pack::Json_Pack(QObject *parent)
{
    mPro = sDataPacket::bulid()->getPro();
}

Json_Pack *Json_Pack::bulid(QObject *parent)
{
    static Json_Pack* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Pack(parent);
    }
    return sington;
}

void Json_Pack::head(QJsonObject &obj)
{
    QDateTime t = QDateTime::currentDateTime();
    mPro->testEndTime = t.toString("yyyy-MM-dd HH:mm:ss");
    mPro->testTime = QString::number(QDateTime::fromString(mPro->testStartTime,"yyyy-MM-dd HH:mm:ss").secsTo(t));
    obj.insert("softwareType", mPro->softwareType);
    obj.insert("productType", mPro->productType);
    obj.insert("productSN", mPro->productSN);
    // obj.insert("macAddress", mPro->macAddress);
    obj.insert("result", mPro->uploadPassResult);
    obj.insert("softwareVersion", mPro->softwareVersion);
    obj.insert("work_order", mPro->clientName);
    obj.insert("companyName", mPro->companyName);
    obj.insert("protocolVersion", mPro->protocolVersion);
    obj.insert("testStartTime", mPro->testStartTime);
    obj.insert("testEndTime", mPro->testEndTime);
    obj.insert("testTime", mPro->testTime);

    pduInfo(obj);
}

void Json_Pack::pduInfo(QJsonObject &obj)
{
    objData(obj);
}



int Json_Pack::objData(QJsonObject &obj)
{
    QJsonArray jsonArray;
    int num = mPro->uploadPass.size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("no",QString::number(i+1));
        subObj.insert("name", mPro->itemName.at(i));
        subObj.insert("result", mPro->uploadPass.at(i)?1:0);

        jsonArray.append(subObj);
    }
    obj.insert("testStep" ,QJsonValue(jsonArray));

    return num;
}

void Json_Pack::getJson(QJsonObject &json , QByteArray &ba)
{
    head(json);
    QJsonDocument jsonDoc(json);
    ba = jsonDoc.toJson();
}
void Json_Pack::http_post(const QString &method, const QString &ip, int port)
{
    QJsonObject json; head(json);

    AeaQt::HttpClient http;
    http.clearAccessCache();
    http.clearConnectionCache();
    QString url = "http://%1:%2/%3";
    http.post(url.arg(ip).arg(port).arg(method))
        .header("content-type", "application/json")
        .onSuccess([&](QString result) {qDebug()<<"result    "<<result;emit httpSig(result,true);})
        .onFailed([&](QString error) {qDebug()<<"error    "<<error;emit httpSig(error,false); })
        .onTimeout([&](QNetworkReply *) {qDebug()<<"http_post timeout    ";emit httpSig("http_post timeout",false); }) // 超时处理
        .timeout(1000) // 1s超时
        .block()
        .body(json)
        .exec();
}

//bool Json_Build::saveJson( QJsonObject &json)
//{
//    QJsonDocument jsonDoc(json);
//    QByteArray ba = jsonDoc.toJson();
//    QString path = CfgCom::bulid()->pathOfData("pdu_id.json");
//    QFile file(path);
//    bool ret = false;
//    if(file.exists())//文件存在则不需要再写
//    {
//        ret = true;
//    }
//    else//文件不存在则写入初始数据
//    {
//        ret = file.open(QIODevice::WriteOnly);
//        if(ret) {
//            file.write(ba);
//            file.close();
//        } else {
//            qDebug() << "write json file failed";
//        }
//    }

//    return ret;
//}

