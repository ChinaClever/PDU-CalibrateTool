/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "sqldatebar.h"
#include "ui_sqldatebar.h"

SqlDateBar::SqlDateBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlDateBar)
{
    ui->setupUi(this);
    on_dayRadio_clicked(true);
}

SqlDateBar::~SqlDateBar()
{
    delete ui;
}


QString SqlDateBar::getDate()
{
    QString str;

    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();
    if(startDate > endDate) {
        CriticalMsgBox box(this, tr("起始日期早于结束日期，请重新填写日期!"));
    } else {
        QString startDateStr = startDate.toString("yyyy-MM-dd");
        QString endDateStr = endDate.toString("yyyy-MM-dd");
        str = QString(" date between \'%1\' and  \'%2\'").arg(startDateStr).arg(endDateStr);
    }

    return str;
}


void SqlDateBar::on_startDateBtn_clicked()
{
    MsgCalendarDlg dlg(this);
    dlg.exec();
    ui->startDateEdit->setDate(dlg.getDate());
}

void SqlDateBar::on_endDateBtn_clicked()
{
    MsgCalendarDlg dlg(this);
    dlg.exec();
    ui->endDateEdit->setDate(dlg.getDate());
}

void SqlDateBar::on_dayRadio_clicked(bool checked)
{
    if(checked) {
        QDate date = QDate::currentDate();
        ui->startDateEdit->setDate(date);
        ui->endDateEdit->setDate(date);
    }
}

void SqlDateBar::on_weekRadio_clicked(bool checked)
{
    if(checked) {
        QDate date = QDate::currentDate();
        ui->startDateEdit->setDate(date.addDays(-7));
        ui->endDateEdit->setDate(QDate::currentDate());
    }
}

void SqlDateBar::on_monthRadio_clicked(bool checked)
{
    if(checked) {
        QDate date = QDate::currentDate();
        ui->startDateEdit->setDate(date.addMonths(-1));
        ui->endDateEdit->setDate(QDate::currentDate());
    }
}

void SqlDateBar::on_currentMonthRadio_clicked(bool checked)
{
    if(checked) {
        QDate date = QDate::currentDate();
        ui->startDateEdit->setDate(date.addDays(1-date.day()));
        ui->endDateEdit->setDate(QDate::currentDate());
    }
}
