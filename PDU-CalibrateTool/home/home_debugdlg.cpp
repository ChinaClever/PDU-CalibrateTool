/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "home_debugdlg.h"
#include "ui_home_debugdlg.h"

Home_DebugDlg::Home_DebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_DebugDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;
    initThread();
}

Home_DebugDlg::~Home_DebugDlg()
{
    delete ui;
}


void Home_DebugDlg::initThread()
{
    sConfigItem *item = AdjustConfig::bulid()->item;
    switch (item->devType) {
    case 1: mCoreThread = Ctrl_ZpduThread::bulid(this); break;
    case 2: mCoreThread = Ctrl_MpduThread::bulid(this); break;
    default:  break;
    }
}

void Home_DebugDlg::on_onAllBtn_clicked()
{
    mCoreThread->openAllSwitch();
}

void Home_DebugDlg::on_eleAllBtn_clicked()
{
    mCoreThread->clearAllEle();
}

void Home_DebugDlg::on_closeAllBtn_clicked()
{
    mCoreThread->closeAllSwitch();
}

void Home_DebugDlg::on_openBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->openOutputSwitch(id);
}

void Home_DebugDlg::on_closeBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->closeOutputSwitch(id);
}

void Home_DebugDlg::on_eleBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->setClearEle(id);
}


void Home_DebugDlg::workDownSlot()
{
    if(++mId <= mData->size) {
        mCoreThread->openOnlySwitch(mId);
        QTimer::singleShot(6*1000,this,SLOT(workDownSlot()));
    }
}

void Home_DebugDlg::on_swAllBtn_clicked()
{
    QuMsgBox box(this, tr("确认是否开启自动切换功能?"));
    if(box.Exec()) {
        mId = 0;
        QTimer::singleShot(50,this,SLOT(workDownSlot()));
    }
}
