/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readmedlg.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupBox_background_icon(this);

    sConfigItem *item = AdjustConfig::bulid()->item;
    mSerialWid = new SerialStatusWid(ui->serialWid);
    item->serial = mSerialWid->initSerialPort(tr("校准源"), 19200);

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    item->source = mSourceWid->initSerialPort(tr("标准源"), 9600);

    mTableWid = new DataTableWid(ui->tableWid);
    mModeWid = new ModeSelectWid(ui->modeWid);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_readmeBtn_clicked()
{
    ReadMeDLg dlg(this);
    dlg.exec();
}
