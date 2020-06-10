#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QtCore>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr);
    static WebSocketClient *bulid(QObject *parent);

    QString getMessage();
    bool sendMessage(const QString &message);
    bool send(const QString &url, int port, const QString &str);

signals:
    void closed();

protected:
    void close();
    void open(const QString &url);
    bool reOpen(const QString &url);
    QString getUrl(const QString &url, int port);

protected slots:
    void openSlot();
    void connected();
    void disconnected();
    void textMessageReceived(const QString &message);

private:
    QString m_url;
    QWebSocket *mSocket;
    bool isConnected, isRun;
    QStringList mRecvList;
};

#endif // WEBSOCKETCLIENT_H
