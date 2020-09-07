/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_debugdlg.h"
#include "ui_home_debugdlg.h"

Home_DebugDlg::Home_DebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_DebugDlg)
{
    ui->setupUi(this);
    initWid();
}

Home_DebugDlg::~Home_DebugDlg()
{
    delete ui;
}

void Home_DebugDlg::initWid()
{
    mThresholdWid = new Home_ThresholdWid(ui->tabWidget);
    ui->tabWidget->addTab(mThresholdWid, tr("设备报警阈值"));

    mOutputWid = new Home_OutputWid(ui->tabWidget);
    ui->tabWidget->addTab(mOutputWid, tr("输出位控制"));
}
