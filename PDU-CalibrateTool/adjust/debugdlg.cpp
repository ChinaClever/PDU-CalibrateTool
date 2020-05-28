#include "debugdlg.h"
#include "ui_debugdlg.h"

DebugDlg::DebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
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
    mCoreThread->clearPduEle();
}
