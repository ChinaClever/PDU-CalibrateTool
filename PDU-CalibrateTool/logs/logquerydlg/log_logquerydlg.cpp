/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "log_logquerydlg.h"
#include "ui_log_logquerydlg.h"

Log_LogQueryDlg::Log_LogQueryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log_LogQueryDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("查询对话框"));
    groupBox_background_icon(this);
    mDateBar = new SqlDateBar(ui->dateWid);
}

Log_LogQueryDlg::~Log_LogQueryDlg()
{
    delete ui;
}

QString Log_LogQueryDlg::getCmd()
{
    QString cmd = mDateBar->getDate();
    QString str = ui->userEdit->text();
    if(!str.isEmpty()) cmd += QString(" and user like '%%1%'").arg(str);

    str = ui->devtypeEdit->text();
    if(!str.isEmpty()) cmd += QString(" and dev like '%%1%'").arg(str);

    str = ui->snEdit->text();
    if(!str.isEmpty()) cmd += QString(" and sn like '%%1%'").arg(str);

    str = ui->statusEdit->text();
    if(!str.isEmpty()) cmd += QString(" and result like '%%1%'").arg(str);

    return cmd;
}


void Log_LogQueryDlg::on_quitBtn_clicked()
{
    close();
}

bool Log_LogQueryDlg::inputCheck()
{
    bool ret = true;

    QString str = ui->snEdit->text();
    if(!str.isEmpty()) {
        if(str.size() != 14) ret = false;
        if(!ret) {
            CriticalMsgBox box(this, tr("设备序列号输入有误，请重新输入!!!"));
        }
    }

    return ret;
}

void Log_LogQueryDlg::on_okBtn_clicked()
{
    if(inputCheck()) {
        this->accept();
    }
}
