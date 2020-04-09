#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readmedlg.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSerialWid = new SerialStatusWid(ui->serialWid);
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
