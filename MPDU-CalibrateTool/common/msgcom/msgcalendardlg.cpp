/*
 * logcalendardlg.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "msgcalendardlg.h"
#include "ui_msgcalendardlg.h"

MsgCalendarDlg::MsgCalendarDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MsgCalendarDlg)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("日期选择"));
    groupBox_background_icon(this);
    connect(ui->calendarWidget,SIGNAL(selectionChanged()),this, SLOT(selectionChangedSLot()));
    selectionChangedSLot();
}

MsgCalendarDlg::~MsgCalendarDlg()
{
    delete ui;
}


void MsgCalendarDlg::selectionChangedSLot()
{
    mDate= ui->calendarWidget->selectedDate();

    QString str = mDate.toString("yyyy-MM-dd");
    ui->timeLab->setText(str);
}


void MsgCalendarDlg::on_pushButton_clicked()
{
    mDate= ui->calendarWidget->selectedDate();
    this->accept();
}
