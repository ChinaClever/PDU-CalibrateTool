/*
 * 对串口的常用操作：打开、读、写、关闭
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "serialport.h"
#include <QSerialPortInfo>
#include <QApplication>
#define SERIAL_TIMEOUT  100  // 100MS

SerialPort::SerialPort(QObject *parent) : QThread(parent)
{
    mCount = 0;
    isOpen = false;
    mSerial = NULL;

    timer = new QTimer(this);
    timer->start(SERIAL_TIMEOUT);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

SerialPort::~SerialPort()
{
    close();
    // delete mSerial;
}

/**
 * @brief 打开串口
 * @param name 串口名  大写
 * @param baudRate 波特率
 * @return true
 */
bool SerialPort::open(const QString &name,qint32 baudRate)
{
    bool ret=false;

    if(!isOpen)
    {
        mSerial = new QSerialPort(name);       //串口号，一定要对应好，大写！！！
        ret = mSerial->open(QIODevice::ReadWrite);      //读写打开
        if(ret)
        {
            mSerial->setBaudRate(baudRate);  //波特率
            mSerial->setDataBits(QSerialPort::Data8); //数据位
            mSerial->setParity(QSerialPort::NoParity);    //无奇偶校验
            mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
            mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
            // connect(mSerial,SIGNAL(readyRead()),this,SLOT(serialReadSlot()));    //连接槽
            isOpen = true;
        }
        else
            qDebug() << "open Serial:" << name << "Err";
    }

    return ret;
}


/**
 * @brief 串口关闭
 * @return true
 */
bool SerialPort::close(void)
{
    if(isOpen)
    {
        QWriteLocker locker(&mRwLock); // 正在操作时不允许关闭
        isOpen = false;
        mSerial->close();
    }

    return true;
}


/**
 * @brief 获取端口名称
 * @return
 */
QString SerialPort::getSerialName()
{
    QString com;

    if(mSerial)
        com =  mSerial->portName();

    return com;
}


/**
 * @brief 获取串口列表
 * @return
 */
QStringList SerialPort::getList()
{
    QStringList list;
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(!infos.isEmpty())
    {
        foreach (QSerialPortInfo info, infos) {
            list <<  info.portName();
        }
    }

    return list;
}

bool SerialPort::isContains(const QString &name)
{
    return getList().contains(name);
}


void SerialPort::timeoutDone()
{
    if(mCount++ % 10) {
        recvSlot();
    } else {
        writeSlot();
    }
}


int SerialPort::send(const QByteArray &array)
{
    int len=0;

    if(isOpen) {
        len = mSerial->write(array);
        if(len > 0) {
            mSerial->flush();
        }
    }

    return len;
}


/**
 * @brief 写数据
 * @param array
 * @return
 */
int SerialPort::write(const QByteArray &array)
{
    int ret = array.size();
    if(isOpen) {
        //QWriteLocker locker(&mRwLock);
        mWriteArrays.append(array);
    } else {
        ret = 0;
        mWriteArrays.clear();
    }

    return ret;
}

int SerialPort::write(uchar *sent, int len)
{
    QByteArray witeArray;
    witeArray.append((char *)sent, len);

    return write(witeArray);
}

void SerialPort::writeSlot()
{
    if(mWriteArrays.size()) {
        int ret = send(mWriteArrays.first());
        if(ret > 0) {
            mWriteArrays.removeFirst();
            mSerialData.clear();
        } else {
            mWriteArrays.clear();
        }
    }
}

/**
 * @brief 读串口
 * @param array
 * @return 长度
 */
void SerialPort::recvSlot()
{
    if(isOpen) {
        /* 处理所有还没有被处理的各类事件，主要是不用用户感觉到ka */
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        QByteArray dataTemp;
        while (!mSerial->atEnd()) {
            dataTemp += mSerial->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据
        }

        // 过滤单片机开机发出0xff无效数据
        if(dataTemp.size() == 1)  {
           if(dataTemp.at(0) == (char)0xFF) dataTemp.clear();
        }

        mSerialData += dataTemp;
    }
}


int SerialPort::read(QByteArray &array, int secs)
{
    if(isOpen) {
        for(int i=0; i<10*secs; ++i) {
            int rtn = mSerialData.size();
            if(rtn > 0) {
                msleep(450);
                QWriteLocker locker(&mRwLock);
                array += mSerialData;
                mSerialData.clear();
                break;
            } else {
                msleep(SERIAL_TIMEOUT);
            }
        }
    }

    return array.size();
}

int SerialPort::read(uchar *recvstr, int secs)
{
    QByteArray array;
    int ret = read(array, secs);
    if(ret < SERIAL_LEN) {
        for(int i=0; i<ret; ++i)
            recvstr[i] = array.at(i);
    } else {
        qDebug() << "SerialPort read too long :" << ret;
        ret = 0;
    }

    return ret;
}

void SerialPort::reflush()
{
    QWriteLocker locker(&mRwLock);
    mSerialData.clear();
}

/**
 * @brief 半双工传输
 * @param witeArray 要发送的数据
 * @param readArray 接收到的数据
 * @return 收到的数据长度
 */
int SerialPort::transmit(const QByteArray &witeArray, QByteArray &readArray, int msecs)
{
    int ret = write(witeArray);
    if(ret > 0) {
        ret = read(readArray, msecs);
        if((ret < 0) || (ret > SERIAL_LEN)) {
            qDebug() << "SerialPort transmit read err"  << ret;
            ret = 0;
        } else if(ret == 0) {
            // qDebug() << "SerialPort transmit read no data"  << ret;
        }
    }

    return ret;
}



/**
  * 功　能：传输数据
  * 入口参数：sent -> 发送缓冲区, len ->  发送长度
  * 出口参数：recv -> 接收缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int SerialPort::transmit(uchar *sent, int len, uchar *recv, int msecs)
{
    QByteArray witeArray, readArray;
    witeArray.append((char *)sent, len);

    int ret = transmit(witeArray, readArray, msecs);
    if(ret > 0) {
        for(int i=0; i<ret; ++i){
            recv[i] = readArray.at(i);
        }
    }
    return ret;
}

bool SerialPort::setBaudRate(qint32 baudRate)
{
    bool ret = false;
    if(isOpen) {
        ret = mSerial->setBaudRate(baudRate);
    }

    return ret;
}

qint32 SerialPort::baudRate()
{
    qint32 ret = 0;
    if(isOpen) {
        ret = mSerial->baudRate();
    }

    return ret;
}

/**
 * @brief 回环测试
 */
bool SerialPort::loopTest()
{
    QByteArray sentArray,recvArray;

    for(int i=0; i<255;++i)
        sentArray.append(i);

    int ret = transmit(sentArray, recvArray);
    if(ret != sentArray.size())
        qDebug() << "Serial test Err:" <<ret;
    else
        qDebug() << "Serial test OK";
    return ret;
}
