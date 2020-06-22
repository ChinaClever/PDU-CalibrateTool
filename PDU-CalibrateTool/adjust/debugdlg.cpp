/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "debugdlg.h"
#include "ui_debugdlg.h"

DebugDlg::DebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;

    sConfigItem *item = AdjustConfig::bulid()->item;
    if(item->devType) {
        mCoreThread = AdjustZpduThread::bulid(this);
    } else {
        mCoreThread =  AdjustMpduThread::bulid(this);
    }
}

DebugDlg::~DebugDlg()
{
    delete ui;
}

void DebugDlg::on_onAllBtn_clicked()
{
    mCoreThread->openAllSwitch();
}

void DebugDlg::on_eleAllBtn_clicked()
{
    mCoreThread->clearAllEle();
}

void DebugDlg::on_closeAllBtn_clicked()
{
    mCoreThread->closeAllSwitch();
}

void DebugDlg::on_openBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->openOutputSwitch(id);
}

void DebugDlg::on_closeBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->closeOutputSwitch(id);
}

void DebugDlg::on_eleBtn_clicked()
{
    int id = ui->spinBox->value();
    mCoreThread->setClearEle(id);
}


void DebugDlg::workDownSlot()
{
    if(++mId <= mData->size) {
        mCoreThread->openOnlySwitch(mId);
        QTimer::singleShot(6*1000,this,SLOT(workDownSlot()));
    }
}

void DebugDlg::on_swAllBtn_clicked()
{
    QuMsgBox box(this, tr("确认是否开启自动切换功能?"));
    if(box.Exec()) {
        mId = 0;
        QTimer::singleShot(50,this,SLOT(workDownSlot()));
    }
}
