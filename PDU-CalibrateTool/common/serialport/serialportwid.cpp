/*
 * 串口窗口
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "serialportwid.h"
#include "ui_serialportwid.h"
#include "msgbox.h"

SerialPortWid::SerialPortWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortWid)
{
    ui->setupUi(this);
    mSerialPort = new SerialPort(this);
    QTimer::singleShot(50,this,SLOT(statusSlot())); //延时初始化

    initSerialPort();
    initWidget();
}

SerialPortWid::~SerialPortWid()
{
    delete ui;
}


void SerialPortWid::initSerialPort()
{
    QStringList com = mSerialPort->getList(); // 端口列表
    ui->comBox->clear();
    ui->comBox->addItems(com);
}


void SerialPortWid::statusSlot()
{
    bool ret = mSerialPort->isOpened();
    if(ret) {
        QString name = mSerialPort->getSerialName(); // 已打开端口
        if(!name.isEmpty()) ui->comBox->setCurrentText(name);

        ui->workBtn->setText(tr("关闭串口"));
        ui->stateLab->setText(tr("%1已打开").arg(name));
    }  else {
        ui->workBtn->setText(tr("打开串口"));
        ui->stateLab->setText(tr("串口未打开"));
    }

    ui->comBox->setDisabled(ret);
    ui->baudComBox->setDisabled(ret);
}

void SerialPortWid::initWidget()
{
    ui->baudComBox->setCurrentIndex(1);
    connect(ui->workBtn,SIGNAL(clicked()),this,SLOT(workBtnSlot()));
    connect(ui->updateBtn,SIGNAL(clicked()),this,SLOT(updateBtnSlot()));
}


void SerialPortWid::openSerialSlot()
{
    QString com = ui->comBox->currentText();
    if(!com.isEmpty()) {
        qint32 baudRate = ui->baudComBox->currentText().toInt();
        bool ret = mSerialPort->open(com, baudRate);
        if(!ret) {
            CriticalMsgBox box(this, tr("打开%1失败").arg(com));
        }
    } else
        CriticalMsgBox box(this, tr("没有串口"));
}


void SerialPortWid::workBtnSlot()
{
    if(mSerialPort->isOpened())   {
        mSerialPort->close();
    } else {
        openSerialSlot();
    }

    statusSlot();
}


void SerialPortWid::updateBtnSlot()
{
    initSerialPort();
    statusSlot();
    InfoMsgBox box(this, tr("串口刷新成功"));
}

void SerialPortWid::on_loopBtn_clicked()
{
    if(mSerialPort->isOpened())
    {
        bool ret = mSerialPort->loopTest();
        if(ret) {
            InfoMsgBox box(this, tr("串口:%1回环测试成功").arg(mSerialPort->getSerialName()));
        } else {
            CriticalMsgBox box(this, tr("串口:%1回环测试失败").arg(mSerialPort->getSerialName()));
        }
    } else {
        CriticalMsgBox box(this, tr("请先打开串口"));
    }
}

