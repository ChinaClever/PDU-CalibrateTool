/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_setdlg.h"
#include "ui_home_setdlg.h"

Home_SetDlg::Home_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_SetDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mItem = Ad_Config::bulid()->item;
    QTimer::singleShot(500,this,SLOT(initFunSlot()));
}

Home_SetDlg::~Home_SetDlg()
{
    delete ui;
}

void Home_SetDlg::initFunSlot()
{
    ui->userEdit->setText(mItem->user);
    ui->numSpin->setValue(mItem->cnt.num);
    QString str = tr("数量:%1 \t成功:%2 \t失败:%3 ")
            .arg(mItem->cnt.all).arg(mItem->cnt.ok).arg(mItem->cnt.err);
    ui->cntLab->setText(str);
}

void Home_SetDlg::on_okBtn_clicked()
{    
    this->close();    
    mItem->user = ui->userEdit->text();
    mItem->cnt.num = ui->numSpin->value();
    Ad_Config::bulid()->writeCnt();
}

void Home_SetDlg::on_resBtn_clicked()
{
    QuMsgBox box(this, tr("请确认，重新计数？"));
    if(box.Exec()) {
        mItem->cnt.all = 0;
        mItem->cnt.ok = 0;
        mItem->cnt.err = 0;
        Ad_Config::bulid()->writeCnt();
    }
}

