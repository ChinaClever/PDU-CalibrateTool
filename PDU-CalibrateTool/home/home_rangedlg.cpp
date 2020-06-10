/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "home_rangedlg.h"
#include "ui_home_rangedlg.h"

Home_RangeDlg::Home_RangeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_RangeDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
}

Home_RangeDlg::~Home_RangeDlg()
{
    delete ui;
}

void Home_RangeDlg::initData(sConfigItem *item)
{
    item->vol = ui->volBox->value() ;
    item->cur = ui->curBox->value() ;
    item->volErr = ui->volErrBox->value() ;
    item->curErr = ui->curErrBox->value() ;
    if(ui->allRadio->isChecked()) {
        item->vert = 1;
    } else {
        item->vert = 0;
    }
}

void Home_RangeDlg::setModel(int index)
{
    if(index) {
        ui->oneRradio->setChecked(true);
        ui->groupBox_2->setEnabled(false);
    } else {
        ui->oneRradio->setChecked(true);
        ui->groupBox_2->setEnabled(true);
    }
}

void Home_RangeDlg::on_okBtn_clicked()
{
    sConfigItem *item = AdjustConfig::bulid()->item;
    initData(item);
    close();
}
