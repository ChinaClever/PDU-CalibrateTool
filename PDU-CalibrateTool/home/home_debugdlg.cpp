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
    groupBox_background_icon(this);
    mData = sDataPacket::bulid()->data;
    initThread();
}

Home_DebugDlg::~Home_DebugDlg()
{
    delete ui;
}


void Home_DebugDlg::initThread()
{
    sDevType *dt = sDataPacket::bulid()->devType;
    switch (dt->devType) {
    case 1: mThread = Ctrl_ZpduThread::bulid(this); break;
    case 2: mThread = Ctrl_MpduThread::bulid(this); break;
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

    int id = ui->spinBox->value();
    if(mThread)
        mThread->openOutputSwitch(id);
}

void Home_DebugDlg::on_closeBtn_clicked()
{
    int id = ui->spinBox->value();
    if(mThread)
        mThread->closeOutputSwitch(id);
}

void Home_DebugDlg::on_eleBtn_clicked()
{
    int id = ui->spinBox->value();
    if(mThread)
        mThread->setClearEle(id);
}


void Home_DebugDlg::workDownSlot()
{
    if(++mId <= mData->size) {
        mThread->openOnlySwitch(mId);
        QTimer::singleShot(6*1000,this,SLOT(workDownSlot()));
    }
}

void Home_DebugDlg::on_swAllBtn_clicked()
{
    if(!mThread) return;
    QuMsgBox box(this, tr("确认是否开启自动切换功能?"));
    if(box.Exec()) {
        mId = 0;
        QTimer::singleShot(50,this,SLOT(workDownSlot()));
    }
}
