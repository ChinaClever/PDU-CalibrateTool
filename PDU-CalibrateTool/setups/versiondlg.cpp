#include "versiondlg.h"
#include "ui_versiondlg.h"
#include <QtCore>
#include "common.h"

VersionDlg::VersionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDlg)
{
    ui->setupUi(this);
    init();
    groupBox_background_icon(this);
}

VersionDlg::~VersionDlg()
{
    delete ui;
}

void VersionDlg::init()
{
    QString str="V0.1版本实现功能：\n    1.ZPDU普通版本的校准；\n    2.MPDU磁保持版本的校准；\n\r";
    ui->textEdit->setText(str);
}

void VersionDlg::on_okBtn_clicked()
{
    this->close();
}
