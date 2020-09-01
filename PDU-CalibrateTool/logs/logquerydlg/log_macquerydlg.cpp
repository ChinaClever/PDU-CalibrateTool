/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "log_macquerydlg.h"
#include "ui_log_macquerydlg.h"
#include "ad_macaddr.h"

Log_MacQueryDlg::Log_MacQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_MacQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);
}

Log_MacQueryDlg::~Log_MacQueryDlg()
{
    delete ui;
}

QString Log_MacQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->userEdit->text();
    if(!str.isEmpty()) cmd += QString(" and user like '%%1%'").arg(str);

    str = ui->snEdit->text();
    if(!str.isEmpty()) cmd += QString(" and sn like '%%1%'").arg(str);

    str = ui->macEdit->text();
    if(!str.isEmpty()) cmd += QString(" and mac like '%%1%'").arg(str);

    int ret = ui->devCmb->currentIndex();
    if(ret) {
        str = ui->devCmb->currentText();
        cmd += QString(" and dev like '%%1%'").arg(str);
    }

    return cmd;
}


void Log_MacQueryDlg::on_quitBtn_clicked()
{
    close();
}

bool Log_MacQueryDlg::inputCheck()
{
    bool ret = true;

    QString str = ui->macEdit->text();
    if(!str.isEmpty()) {
        ret = Ad_MacAddr::bulid()->isMacAddress(str);
        if(!ret) {
            CriticalMsgBox box(this, tr("设备MAC地址输入有误，请重新输入!!!"));
        }
    }

    return ret;
}

void Log_MacQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}
