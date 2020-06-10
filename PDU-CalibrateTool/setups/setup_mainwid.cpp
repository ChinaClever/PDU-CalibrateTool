/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "logmainwid.h"
#include "configbase.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(rand()%50,this,SLOT(initFunSLot()));
}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}

void Setup_MainWid::initFunSLot()
{
    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);

//    sConfigItem *item = ConfigBase::bulid()->item;
//    ui->logCountSpin->setValue(item->logCount/10000);
}

void Setup_MainWid::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWid->setCurrentIndex(index);
}


void Setup_MainWid::on_logCountSpin_valueChanged(int arg1)
{
//    ConfigBase *con = ConfigBase::bulid();
//    con->item->logCount = arg1*10000;
//    con->setLogCount(arg1*10000);
}
