/*
 *
 *
 *  Created on: 2020年3月9日
 *      Author: Lzy
 */
#include "json_recv.h"

Json_Recv::Json_Recv(QObject *parent) : QObject(parent)
{
    mJsonSize = SN_TOTAL_NUM;
}

Json_Recv::~Json_Recv()
{

}

Json_Recv *Json_Recv::bulid(QObject *parent)
{
    static Json_Recv* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Recv(parent);
    }
    return sington;
}

QJsonValue Json_Recv::getValue(QJsonObject &object, const QString &key)
{
    QJsonValue value;
    if (object.contains(key))  {
        value = object.value(key);
    }
    return value;
}

QString Json_Recv::getString(QJsonObject &object, const QString &key)
{
    QString str;
    QJsonValue value = getValue(object, key);
    if(value.isString()) {
        str = value.toString();
    }

    return str;
}

double Json_Recv::getData(QJsonObject &object, const QString &key)
{
    double ret = -1;
    QJsonValue value = getValue(object, key);
    if(value.isDouble()) {
        ret = value.toDouble();
    }

    return ret;
}


QJsonObject Json_Recv::getObject(QJsonObject &object, const QString &key)
{
    QJsonObject obj;
    if (object.contains(key)){
        QJsonValue value = object.value(key);
        if (value.isObject()){
            obj = value.toObject();
        }
    }
    return obj;
}

QJsonArray Json_Recv::getArray(QJsonObject &object, const QString &key)
{
    QJsonArray array;
    if (object.contains(key)) {
        QJsonValue value = object.value(key);
        if (value.isArray()) {
            array = value.toArray();
        }
    }

    return array;
}


bool Json_Recv::company(QJsonObject &object)
{
    bool ret = false;
    QString strName = getString(object, "company");
    ret = strName.contains("clever");

    return ret;
}


bool Json_Recv::versionNumber(QJsonObject &object)
{
    bool ret = company(object);
    if(ret) {
        mJsonSize = getData(object, "version");
    }

    return ret;
}



bool Json_Recv::recv(const QString &name)
{
    bool ret = false;
    QByteArray ba;
    QString path = CfgCom::bulid()->pathOfData(QString("%1.json").arg(name));
    QFile file(path);
    ret = file.open(QIODevice::ReadOnly);
    if(file.exists())
    {
        if(ret)
        {
            ba = file.readAll();
            file.close();
            ret = true;
            QJsonParseError jsonerror;
            QJsonDocument doc = QJsonDocument::fromJson(ba, &jsonerror);
            if (!doc.isNull() && jsonerror.error == QJsonParseError::NoError) {
                if(doc.isObject()) {
                    QJsonObject object = doc.object();
                    if(versionNumber(object)) {
                        ret = analyticalData(object,Json_Build::bulid()->mProduct);
                    }
                }
            }
        }
        else
            qDebug()<<"open file error!"<<endl;
    }


    return ret;
}

bool Json_Recv::analyticalData(QJsonObject &object ,QVector<QString>& mProduct)
{
    int num = mProduct.size();
    for(int j = 0 ; j < num ; j++)
    {
        QJsonArray array = getArray(object, mProduct[j]);
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i)
        {
            QJsonObject obj = array.at(i).toObject();
            uint id = getData(obj, "id");
            QString strNo = getString(obj, "SN_NO");
            QString strDevType = getString(obj, "dev_type");
            //qDebug()<<"id----"<<id<<"strNo----"<<strNo<<"strDevType----"<<strDevType<<endl;
            mIdToDevType.insert(id,strDevType);
        }
    }

    return true;
}

QString Json_Recv::getDevTypeByID(uint id)
{
    return mIdToDevType[id];
}
