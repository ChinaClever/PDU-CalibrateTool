/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_debugdlg.h"
#include "ui_home_debugdlg.h"
#include "yc_dc107.h"

Home_DebugDlg::Home_DebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_DebugDlg)
{
    ui->setupUi(this);
    this->setEnabled(false);
    groupBox_background_icon(this);
    mData = sDataPacket::bulid()->data;
    QTimer::singleShot(1*1000,this,SLOT(initFunSlot()));
}

Home_DebugDlg::~Home_DebugDlg()
{
    delete ui;
}

void Home_DebugDlg::initFunSlot()
{
    sDevType *dt = sDataPacket::bulid()->devType;
    if(dt->devType > RPDU) {
        return;
    } else if(dt->devType && mData->size) {
        initThread();
        this->setEnabled(true);
    } else {
        QTimer::singleShot(1*1000,this,SLOT(initFunSlot()));
    }
}

void Home_DebugDlg::initThread()
{
    sDevType *dt = sDataPacket::bulid()->devType;
    switch (dt->devType) {
    case ZPDU: mThread = Ctrl_ZpduThread::bulid(this); break;
    case MPDU: mThread = Ctrl_MpduThread::bulid(this); break;
    case RPDU: mThread = Ctrl_RpduThread::bulid(this); break;
    default:  break;
    }
}

void Home_DebugDlg::on_onAllBtn_clicked()
{
    if(mThread)
        mThread->openAllSwitch();
}

void Home_DebugDlg::on_eleAllBtn_clicked()
{
    if(mThread)
        mThread->clearAllEle();
}

void Home_DebugDlg::on_closeAllBtn_clicked()
{
    if(mThread)
        mThread->closeAllSwitch();
}

void Home_DebugDlg::on_openBtn_clicked()
{
    if(mThread) {
        int id = ui->spinBox->value();
        mThread->openOutputSwitch(id-1);
    }
}

void Home_DebugDlg::on_closeBtn_clicked()
{
    if(mThread) {
        int id = ui->spinBox->value();
        mThread->closeOutputSwitch(id-1);
    }
}

void Home_DebugDlg::on_eleBtn_clicked()
{
    if(mThread) {
        int id = ui->spinBox->value();
        mThread->setClearEle(id-1);
    }
}


void Home_DebugDlg::workDownSlot()
{
    if(mId < mData->size) {
        mThread->openOnlySwSlot(mId++);
        QTimer::singleShot(10*1000,this,SLOT(workDownSlot()));
    }
}

void Home_DebugDlg::on_swAllBtn_clicked()
{
    if(!mThread) return;
    QuMsgBox box(this, tr("确认是否开启自动切换功能?"));
    if(box.Exec()) {
        mId = 0;
        QTimer::singleShot(2*1000,this,SLOT(workDownSlot()));
    }
}


void Home_DebugDlg::on_onBtn_clicked()
{
    QuMsgBox box(this, tr("是否需要校准上电?"));
    if(!box.Exec()) return;

    int ret = YC_Ac92b::bulid(this)->powerOn();
    if(ret <= 0) {
        CriticalMsgBox box(this, tr("标准源上电失败!"));
    }
}

void Home_DebugDlg::on_downBtn_clicked()
{
    QuMsgBox box(this, tr("是否需要校准下电?"));
    if(!box.Exec()) return;

    YC_Ac92b::bulid(this)->powerDown();
}

