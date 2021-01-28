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

    Ad_Config *con = Ad_Config::bulid();
    int value = con->read("ai_mode", 0, "Sys").toInt();
    ui->aiBox->setCurrentIndex(value);
}

void Home_SetDlg::updateSiLed()
{
    mItem->si_led = ui->ledCheck->isChecked();
    int rtn = ui->siLineBox->currentIndex();
    if(rtn) {
        mItem->si_ac = AC;
        mItem->si_line = rtn;
    } else {
        mItem->si_ac = DC;
        mItem->si_line = 1;
    }
}

void Home_SetDlg::on_okBtn_clicked()
{    
    this->close();    
    updateSiLed();
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

void Home_SetDlg::on_aiBox_currentIndexChanged(int index)
{
    mItem->aiMode = index;
    Ad_Config *con = Ad_Config::bulid();
    con->write("ai_mode", index, "Sys");
}
