#ifndef JSON_RECV_H
#define JSON_RECV_H
#include "json_build.h"
#include <QUdpSocket>


class Json_Recv :  public QObject
{
    Q_OBJECT
    explicit Json_Recv(QObject *parent = nullptr);
public:
    ~Json_Recv();
    static Json_Recv *bulid(QObject *parent = nullptr);
    bool recv(const QString &name);

    QString getDevTypeByID(uint id);

protected:
    QJsonValue getValue(QJsonObject &object, const QString &key);
    QJsonObject getObject(QJsonObject &object, const QString &key);
    QJsonArray getArray(QJsonObject &object, const QString &key);
    QString getString(QJsonObject &object, const QString &key);
    double getData(QJsonObject &object, const QString &key);

    bool company(QJsonObject &object);
    bool versionNumber(QJsonObject &object);

    bool analyticalData(QJsonObject &object,QVector<QString>& mProduct);
    int mJsonSize;
    QMap<uint,QString> mIdToDevType;

};

#endif // JSON_RECV_H
