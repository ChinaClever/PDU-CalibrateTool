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
    initThresholdWid();
    Ad_Config::bulid()->writeThreshold();
}

void Home_SetDlg::initThresholdWid()
{
    sConfigThreshold *cth = &(mItem->cTh);
    ui->checkBox->setChecked(cth->repair_en);
    on_checkBox_clicked(cth->repair_en);
    ui->comboBox->setCurrentIndex(cth->type);
    on_comboBox_currentIndexChanged(cth->type);

    ui->ipTypeBox->setCurrentIndex(cth->ip_version);
    ui->languageBox->setCurrentIndex(cth->ip_language);
    ui->lineBox->setCurrentIndex(cth->ip_lines-1);
    ui->ipModeBox->setCurrentIndex(cth->ip_mod);
    ui->sBox->setCurrentIndex(cth->ip_standard);
    ui->logBox->setCurrentIndex(cth->ip_log);
    ui->macEdit->setText(cth->mac_addr);
    on_ipTypeBox_currentIndexChanged(cth->ip_version);
}

bool Home_SetDlg::getThresholdWid()
{
    bool ret = true;
    sConfigThreshold *cth = &(mItem->cTh);
    cth->repair_en = ui->checkBox->isChecked() ? 1:0;
    cth->type = ui->comboBox->currentIndex();
    cth->vol_min = ui->volMinSpin->value();
    cth->vol_max = ui->volMaxSpin->value();
    cth->cur_min = ui->curMinSpin->value()*10;
    cth->cur_max = ui->curMaxSpin->value()*10;
    cth->si_mod = ui->siCheck->isChecked() ? 1:0;
    cth->ip_version = ui->ipTypeBox->currentIndex();
    cth->ip_language = ui->languageBox->currentIndex();
    cth->ip_lines = ui->lineBox->currentIndex()+1;
    cth->ip_mod = ui->ipModeBox->currentIndex();
    cth->ip_standard = ui->sBox->currentIndex();
    cth->ip_log = ui->logBox->currentIndex();

    QString mac = ui->macEdit->text();
    if(!mac.isEmpty()) {
        ret = Ad_MacAddr::bulid()->isMacAddress(mac);
        if(!ret) {
            CriticalMsgBox box(this, tr("MAC地址设置错误"));
        }
    }
    char* ch = mac.toLatin1().data();
    strcpy(cth->mac_addr, ch);

    return ret;
}

void Home_SetDlg::setThresholdWid()
{
    sConfigThreshold *cth = &(mItem->cTh);
    ui->volMinSpin->setValue(cth->vol_min);
    ui->volMaxSpin->setValue(cth->vol_max);
    ui->curMinSpin->setValue(cth->cur_min/10.0);
    ui->curMaxSpin->setValue(cth->cur_max/10.0);
}

void Home_SetDlg::on_comboBox_currentIndexChanged(int index)
{
    bool en = true;
    sConfigThreshold *cth = &(mItem->cTh);
    cth->type = index;
    if(index) {
        cth->vol_min = 80;
        cth->vol_max = 276;
        cth->cur_min = 0;
        switch (index) {
        case 1:  cth->cur_max = 320; break;
        case 2:  cth->cur_max = 160; break;
        case 3:  cth->cur_max = 630; break;
        }
    } else {
        en = false;
    }

    setThresholdWid();
    ui->volMinSpin->setEnabled(en);
    ui->volMaxSpin->setEnabled(en);
    ui->curMinSpin->setEnabled(en);
    ui->curMaxSpin->setEnabled(en);
}

void Home_SetDlg::on_okBtn_clicked()
{
    bool ret = getThresholdWid();
    if(ret) {
        this->close();
        Ad_Config::bulid()->writeThreshold();
    }
}

void Home_SetDlg::on_checkBox_clicked(bool checked)
{
    ui->groupBox_1->setEnabled(!checked);
    ui->groupBox_2->setEnabled(!checked);
}

void Home_SetDlg::on_ipTypeBox_currentIndexChanged(int index)
{
    ui->logBox->setEnabled(!index);
}
