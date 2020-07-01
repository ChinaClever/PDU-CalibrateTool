/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_push.h"
#include "udp/sent/udpsentsocket.h"
#include "json_recv.h"

Json_Push::Json_Push(QObject *parent) : QObject(parent)
{
    Json_Recv::bulid(this);
    mJson = Json_Build::bulid();
    mItem = ConfigBase::bulid()->item;
    mSocket = WebSocketClient::bulid(this);
}

Json_Push *Json_Push::bulid(QObject *parent)
{
    static Json_Push* sington = nullptr;
    if(sington == nullptr)
        sington = new Json_Push(parent);
    return sington;
}

void Json_Push::udpPush(QString &str)
{
    UdpSentSocket *socket = UdpSentSocket::bulid();

    for(int i=0; i<PUSH_UDP_SIZE; ++i) {
        int en = mItem->udpEn[i];
        QString url = mItem->udpUrl[i];
        int port = mItem->udpPort[i];

        if(en && url.size()) {
            socket->sentData(url, str.toUtf8(), port);
        }
    }
}


void Json_Push::wsPush(QString &str)
{
    sConfigItem *item = ConfigBase::bulid()->item;
    if(item->wsEn && item->wsUrl.size()) {
        mSocket->send(item->wsUrl, item->wsPort, str);
    }
}


bool Json_Push::check(int count)
{
    bool ret = true;
    if(count % mItem->pushTime) {
        ret = false;
    }

    return ret;
}

bool Json_Push::push(sDataPacket *packet)
{
    bool ret = check(packet->count);
    if(ret) {
        QJsonObject json;
        ret = mJson->getJson(packet, json);
        if(ret) {
            QJsonDocument doc(json);
            QString str = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
            udpPush(str);
            wsPush(str);
        }
    }
    return ret;
}
