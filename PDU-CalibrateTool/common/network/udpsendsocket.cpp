/*
 * UdpSendSocket.cpp
 * UDP 接收套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsendsocket.h"


/**
 * @brief 构造函数
 * @param parent
 */
UdpSendSocket::UdpSendSocket(QObject *parent) : QThread(parent)
{    
    isRun=false;
    mUdpSocket = NULL;
}

UdpSendSocket::~UdpSendSocket()
{
    isRun=false;
    wait();

    delete mUdpSocket;
}

/**
 * @brief 初始化套接字
 * @return 端口号
 */
bool UdpSendSocket::initSocket(int port)
{
    mUdpSocket = new QUdpSocket(this);
    bool ret = mUdpSocket->bind(QHostAddress::AnyIPv4, port);
    if(ret) {
        start(); // 启动接收线程
    } else
        qDebug() << "udp socket create err" << port;

    return ret;
}


/**
 * @brief 读取socket数据
 * @return
 */
int UdpSendSocket::dataSend(void)
{   
    QJsonObject obj;
    QByteArray ba;
    Json_Pack::bulid()->getJson(obj , ba);
    int ret = mUdpSocket->writeDatagram(ba , QHostAddress::LocalHost , 20015);
    if(ret > 0) mUdpSocket->flush(); msleep(100);
    return ret;
}
