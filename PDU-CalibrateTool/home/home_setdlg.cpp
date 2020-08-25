#include "home_setdlg.h"
#include "ui_home_setdlg.h"

Home_SetDlg::Home_SetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home_SetDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mItem = Ad_Config::bulid()->item;
    initThresholdWid();
}

Home_SetDlg::~Home_SetDlg()
{
    delete ui;
}


void Home_SetDlg::initThresholdWid()
{
    ui->comboBox->setCurrentIndex(mItem->cTh.type);
    on_comboBox_currentIndexChanged(mItem->cTh.type);

    ui->siCheck->setChecked(mItem->cTh.si_mod);
    ui->eleCheck->setChecked(mItem->cTh.ele_clear);
    ui->ipCheck->setChecked(mItem->cTh.ip_mod);
    ui->macCheck->setChecked(mItem->cTh.set_mac);
    ui->macEdit->setText(mItem->cTh.mac_addr);
    ui->clearRadio->setChecked(mItem->cTh.mac_clear);
    on_macCheck_clicked(mItem->cTh.set_mac);
}

void Home_SetDlg::getThresholdWid()
{
    mItem->cTh.type = ui->comboBox->currentIndex();
    mItem->cTh.vol_min = ui->volMinSpin->value();
    mItem->cTh.vol_max = ui->volMaxSpin->value();
    mItem->cTh.cur_min = ui->curMinSpin->value();
    mItem->cTh.cur_max = ui->curMaxSpin->value();

    mItem->cTh.si_mod = ui->siCheck->isChecked() ? 1:0;
    mItem->cTh.ele_clear = ui->eleCheck->isChecked() ? 1:0;
    mItem->cTh.ip_mod = ui->ipCheck->isChecked() ? 1:0;
    mItem->cTh.set_mac = ui->macCheck->isChecked() ? 1:0;
    mItem->cTh.mac_clear = ui->clearRadio->isChecked() ? 1:0;

    char* ch = ui->macEdit->text().toLatin1().data();
    strcpy(mItem->cTh.mac_addr, ch);
}

void Home_SetDlg::setThresholdWid()
{
    ui->volMinSpin->setValue(mItem->cTh.vol_min);
    ui->volMaxSpin->setValue(mItem->cTh.vol_max);
    ui->curMinSpin->setValue(mItem->cTh.cur_min/10.0);
    ui->curMaxSpin->setValue(mItem->cTh.cur_max/10.0);
}


void Home_SetDlg::on_comboBox_currentIndexChanged(int index)
{
    bool en = true;
    mItem->cTh.type = index;
    if(index) {
        mItem->cTh.vol_min = 80;
        mItem->cTh.vol_max = 276;
        mItem->cTh.cur_min = 0;
        switch (index) {
        case 1:  mItem->cTh.cur_max = 320; break;
        case 2:  mItem->cTh.cur_max = 160; break;
        case 3:  mItem->cTh.cur_max = 630; break;
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

void Home_SetDlg::on_macCheck_clicked(bool checked)
{
    ui->macEdit->setEnabled(checked);
    ui->addRadio->setEnabled(checked);
    ui->clearRadio->setEnabled(checked);
}

void Home_SetDlg::on_okBtn_clicked()
{
    this->close();
    getThresholdWid();
    Ad_Config::bulid()->writeThreshold();
}
