/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "logmainwid.h"
#include "ui_logmainwid.h"

LogMainWid::LogMainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogMainWid)
{
    ui->setupUi(this);
    // groupBox_background_icon(this);
    QTimer::singleShot(rand()%50,this,SLOT(initFunSLot()));
}

LogMainWid::~LogMainWid()
{
    delete ui;
}


void LogMainWid::initFunSLot()
{
    mLogWid = new LogComWid(ui->tabWidget);
    mLogWid->initWid(DbLogs::bulid(),new LogBtnBar(),  new Log_LogQueryDlg(this));
    ui->tabWidget->addTab(mLogWid, tr("校准结果日志"));

    mStatusWid = new LogComWid(ui->tabWidget);
    mStatusWid->initWid(DbStatus::bulid(),new LogBtnBar(),  new Log_LogQueryDlg(this));
    ui->tabWidget->addTab(mStatusWid, tr("校准过程日志"));
}

