#include "versiondlg.h"
#include "ui_versiondlg.h"
#include <QtCore>
#include "common.h"

VersionDlg::VersionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
}

VersionDlg::~VersionDlg()
{
    delete ui;
}


void VersionDlg::on_okBtn_clicked()
{
    this->close();
}
