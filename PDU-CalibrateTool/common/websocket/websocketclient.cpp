#include "websocketclient.h"
#include "configbase.h"

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    isRun = false;
    isConnected = false;
    mSocket = new QWebSocket();

    connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
}

WebSocketClient *WebSocketClient::bulid(QObject *parent)
{
    static WebSocketClient* sington = nullptr;
    if(sington == nullptr)
        sington = new WebSocketClient(parent);
    return sington;
}


QString WebSocketClient::getUrl(const QString &url, int port)
{
    QString host = url + ":" + QString::number(port);
    return host;
}


void WebSocketClient::openSlot()
{
     mSocket->open(m_url);
     QCoreApplication::processEvents(QEventLoop::AllEvents,500);
     isRun = false;
}

void WebSocketClient::close()
{
    m_url.clear();
    if(isConnected) {
        mSocket->abort();
        isConnected = false;
        QCoreApplication::processEvents(QEventLoop::AllEvents,500);
    }
}


void WebSocketClient::open(const QString &url)
{
    if(!url.isEmpty()) {
        if(!isRun) {
            isRun = true;
            int t = 1 *1000;
            QTimer::singleShot(t,this,SLOT(openSlot()));
        }
        m_url = url;
    }
}

bool WebSocketClient::reOpen(const QString &url)
{
    if(url != m_url) close();
    if(!isConnected)  open(url);

    return isConnected;
}


bool WebSocketClient::send(const QString &url, int port, const QString &str)
{
    QString host = getUrl(url, port);
    bool ret = reOpen(host);
    if(ret) {
        ret = sendMessage(str);
    }

    return ret;
}


bool WebSocketClient::sendMessage(const QString &message)
{
    bool ret = false;
    if(isConnected) {
        int rtn = mSocket->sendTextMessage(message);
        if(rtn > 0)  ret = true;
        mSocket->flush();
    }

    return ret;
}



void WebSocketClient::connected()
{
    isConnected = true;
}

void WebSocketClient::disconnected()
{
    isConnected = false;
    emit closed();
}

void WebSocketClient::textMessageReceived(const QString &message)
{
//    mRecvList.append(message);
    qDebug() << "WebSocketClient recv" <<message;
}

QString WebSocketClient::getMessage()
{
    QString str = mRecvList.first();
    if(!str.isEmpty()) {
        mRecvList.removeFirst();
    }

    return str;
}
