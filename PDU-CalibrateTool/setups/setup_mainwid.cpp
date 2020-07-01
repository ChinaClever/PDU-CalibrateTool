/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "logmainwid.h"
#include "readmedlg.h"

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
    item->serial = mSerialWid->initSerialPort(tr("校准源"), 19200);

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    item->source = mSourceWid->initSerialPort(tr("标准源"), 9600);
}

void Setup_MainWid::checkPcNumSlot()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    int num = item->pcNum;

    if(num < 1) {
        CriticalMsgBox box(this, tr("请联系研发部设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
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
    int value = (int)con->getValue("pc_num");
    if(value < 1) value = 0;

    sConfigItem *item = con->item;
    item->pcNum = value;
    ui->pcNumSpin->setValue(value);
}


void Setup_MainWid::initLogCount()
{
    Ad_Config *con = Ad_Config::bulid();
    int value = (int)con->getValue("log_count");
    if(value < 1) value = 10*10000;

    sConfigItem *item = con->item;
    item->logCount = value;
    ui->logCountSpin->setValue(value/10000);
}


void Setup_MainWid::writeLogCount()
{
    Ad_Config *con = Ad_Config::bulid();
    int arg1 = ui->logCountSpin->value();
    con->item->logCount = arg1*10000;
    con->setValue(arg1*10000, "log_count");
}

void Setup_MainWid::updateErrData()
{
    sConfigItem *item = Ad_Config::bulid()->item;
    item->volErr = ui->volErrBox->value();
    item->curErr = ui->curErrBox->value() * 10;
}

void Setup_MainWid::initErrData()
{
    Ad_Config *con = Ad_Config::bulid();
    double value = con->getValue("vol_err");
    if(value < 0) value = 1;
    ui->volErrBox->setValue(value);

    value = con->getValue("cur_err");
    if(value < 0) value = 0.1;
    ui->curErrBox->setValue(value);

    updateErrData();
}

void Setup_MainWid::writeErrData()
{
    Ad_Config *con = Ad_Config::bulid();
    double value = ui->volErrBox->value();
    con->setValue(value, "vol_err");

    value = ui->curErrBox->value();
    con->setValue(value, "cur_err");

    updateErrData();
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
        writeErrData();
        updateErrData();
    } else {
        str = tr("保存");
    }

    ui->saveBtn->setText(str);
    ui->volErrBox->setEnabled(ret);
    ui->curErrBox->setEnabled(ret);
}

void Setup_MainWid::writePcNum()
{
    Ad_Config *con = Ad_Config::bulid();
    int arg1 = ui->pcNumSpin->value();
    con->item->pcNum = arg1;
    con->setValue(arg1, "pc_num");
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
