/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "readmedlg.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mNavBarWid = new NavBarWid(ui->barWid);
    QTimer::singleShot(5,this,SLOT(initFunSlot())); //延时初始化
    connect(mNavBarWid, SIGNAL(navBarSig(int)), this, SLOT(navBarSlot(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initFunSlot()
{
    initWid();
}


void MainWindow::initWid()
{
    mHomeWid = new Home_MainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mHomeWid);

    mSetupWid = new Setup_MainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mSetupWid);

    mLog = new LogMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLog);
}

void MainWindow::navBarSlot(int id)
{
    ui->stackedWid->setCurrentIndex(id);
}



void MainWindow::on_readmeBtn_clicked()
{
//    ReadMeDLg dlg(this);
//    dlg.exec();
}
