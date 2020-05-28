#include "errorrangedlg.h"
#include "ui_errorrangedlg.h"

ErrorRangeDlg::ErrorRangeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorRangeDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
}

ErrorRangeDlg::~ErrorRangeDlg()
{
    delete ui;
}

void ErrorRangeDlg::initData(sConfigItem *item)
{
    item->vol = ui->volBox->value() * 10;
    item->cur = ui->curBox->value() * 10;
    item->volErr = ui->volErrBox->value() * 10;
    item->curErr = ui->curErrBox->value() * 10;
    if(ui->allRadio->isChecked()) {
        item->vert = 1;
    } else {
        item->vert = 0;
    }
}

void ErrorRangeDlg::setModel(int index)
{
    if(index) {
        ui->oneRradio->setChecked(true);
        ui->groupBox_2->setEnabled(false);
    } else {
        ui->allRadio->setChecked(true);
        ui->groupBox_2->setEnabled(true);
    }
    ui->groupBox_2->setEnabled(false); // 不允许选择
}

void ErrorRangeDlg::on_okBtn_clicked()
{
    sConfigItem *item = AdjustConfig::bulid()->item;
    initData(item);
    close();
}
