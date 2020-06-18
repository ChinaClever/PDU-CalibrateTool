/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_mainwid.h"
#include "ui_home_mainwid.h"

Home_MainWid::Home_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mModeWid = new Home_WorkWid(ui->modeWid);
    mTableWid = new Home_DataTabWid(ui->tableWid);
}

Home_MainWid::~Home_MainWid()
{
    delete ui;
}


