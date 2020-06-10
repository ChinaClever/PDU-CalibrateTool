/*
 * log_exportdlg.cpp
 * 导出窗口
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "sqlexportdlg.h"
#include "ui_sqlexportdlg.h"

SqlExportDlg::SqlExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlExportDlg)
{

    ui->setupUi(this);
    groupBox_background_icon(this);
    this->setWindowTitle(tr("日志导出"));
    mExportThread = new Excel_SaveThread(this);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

SqlExportDlg::~SqlExportDlg()
{
    delete ui;
}

void SqlExportDlg::init(const QString &title, QList<QStringList> &list)
{
    mList = list;
    QString fn = title + tr("导出");

    ui->progressBar->setValue(0);
    ui->titleLab->setText(fn);
    ui->fileEdit->setText(fn);
}

/**
 * @brief 路径选择
 */
void SqlExportDlg::on_pushButton_clicked()
{
    QFileDialog dlg(this,tr("路径选择"));
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    dlg.setDirectory("E:");
    if(dlg.exec() == QDialog::Accepted) {
        QString fn = dlg.selectedFiles().at(0);
        if(fn.right(1) != "/")  fn += "/";
        ui->pathEdit->setText(fn);
    }
}


/**
 * @brief 检查输入
 */
bool SqlExportDlg::checkInput()
{
    QString str = ui->pathEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出路径不能为空！"));
        return false;
    }

    str = ui->fileEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出文件名不能为空！"));
        return false;
    }

    str = ui->pathEdit->text() + ui->fileEdit->text() +".xlsx";
    QFile file(str);
    if (file.exists()){
        CriticalMsgBox box(this, str + tr("\n文件已存在！!"));
        return false;
    }
    ui->exportBtn->setEnabled(false);
    ui->quitBtn->setEnabled(false);
    ui->progressBar->setValue(1);

    return true;
}

/**
 * @brief 导出完成
 */
void SqlExportDlg::exportDone()
{
    ui->exportBtn->setEnabled(true);
    ui->quitBtn->setEnabled(true);
    InfoMsgBox box(this, tr("\n导出完成!!\n"));
}

void SqlExportDlg::timeoutDone()
{
    int progress = mExportThread->getProgress();
    if(progress < 100)
        ui->progressBar->setValue(progress);
    else {
        ui->progressBar->setValue(progress);
        timer->stop();
        exportDone();
    }
}


void SqlExportDlg::on_exportBtn_clicked()
{
    bool ret = checkInput();
    if(ret) {
        timer->start(100);
        QString fn = ui->pathEdit->text() + ui->fileEdit->text();
        mExportThread->saveData(fn, mList);
    }
}


void SqlExportDlg::on_quitBtn_clicked()
{
    if(timer->isActive()) {
        InfoMsgBox box(this, tr("\n导出还没有完成，还不能关闭!!\n"));
    } else   {
        this->close();
    }
}
