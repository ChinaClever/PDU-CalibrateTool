/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "logmainwid.h"
#include "versiondlg.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(rand()%13,this,SLOT(initFunSlot()));
    initSerial();
}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}

void Setup_MainWid::initSerial()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    mSerialWid = new SerialStatusWid(ui->serialWid);
    item->serial = mSerialWid->initSerialPort(tr("PDU"));

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    item->source = mSourceWid->initSerialPort(tr("标准源"));
}

void Setup_MainWid::checkPcNumSlot()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    int num = item->pcNum;

    if(num < 1) {
        if(!usr_land_jur())
            CriticalMsgBox box(this, tr("请联系研发部设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
        else
            CriticalMsgBox box(this, tr("请自行设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
        QTimer::singleShot(20*1000,this,SLOT(checkPcNumSlot()));
    }
}

void Setup_MainWid::initFunSlot()
{
    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);

    initPcNum();
    initLogCount();
    initErrData();

    QTimer::singleShot(2*1000,this,SLOT(checkPcNumSlot()));
}

void Setup_MainWid::initPcNum()
{
    Ad_Config *con = Ad_Config::bulid();
    int value = con->read("pc_num", 0, "Sys").toInt();

    sConfigItem *item = con->item;
    item->pcNum = value;
    ui->pcNumSpin->setValue(value);
}


void Setup_MainWid::initLogCount()
{
    Ad_Config *con = Ad_Config::bulid();
    int value = con->read("log_count", 10, "Sys").toInt();

    sConfigItem *item = con->item;
    item->logCount = value * 10000;
    ui->logCountSpin->setValue(value);
}


void Setup_MainWid::writeLogCount()
{
    Ad_Config *con = Ad_Config::bulid();
    int arg1 = ui->logCountSpin->value();
    con->item->logCount = arg1;
    con->write("log_count", arg1, "Sys");
}

void Setup_MainWid::updateErrData()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    item->volErr = ui->volErrBox->value();
    item->curErr = ui->curErrBox->value() * 10;
    item->powErr = ui->powErrBox->value() * 10;

    Ad_Config::bulid()->writeErrData();
}

void Setup_MainWid::initErrData()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    ui->volErrBox->setValue(item->volErr);
    ui->curErrBox->setValue(item->curErr / 10.0);
    ui->powErrBox->setValue(item->powErr / 10.0);
}



void Setup_MainWid::on_saveBtn_clicked()
{
    static int flg = 0;
    QString str = tr("修改");

    bool ret = usr_land_jur();
    if(!ret) {
        CriticalMsgBox box(this, tr("你无权进行此操作"));
        return;
    }

    if(flg++ %2) {
        ret = false;
        updateErrData();
    } else {
        str = tr("保存");
    }

    ui->saveBtn->setText(str);
    ui->volErrBox->setEnabled(ret);
    ui->curErrBox->setEnabled(ret);
    ui->powErrBox->setEnabled(ret);
}

void Setup_MainWid::writePcNum()
{
    Ad_Config *con = Ad_Config::bulid();
    int arg1 = ui->pcNumSpin->value();
    con->item->pcNum = arg1;
    con->write("pc_num", arg1, "Sys");
}

void Setup_MainWid::on_pcBtn_clicked()
{
    static int flg = 0;
    QString str = tr("修改");

    bool ret = usr_land_jur();
    if(!ret) {
        CriticalMsgBox box(this, tr("你无权进行此操作"));
        return;
    }

    if(flg++ %2) {
        ret = false;
        writePcNum();
        writeLogCount();
    } else {
        str = tr("保存");
    }

    ui->pcBtn->setText(str);
    ui->pcNumSpin->setEnabled(ret);
    ui->logCountSpin->setEnabled(ret);
}

void Setup_MainWid::on_pushButton_clicked()
{
    VersionDlg dlg(this);
    dlg.exec();
}
