/*
 * 日志窗口公共基类
 *      只需要实现二个纯虚函数即可
 *       virtual BasicSql *getDb()=0;
         virtual void initTable()=0;
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "logcomwid.h"
#include "ui_logcomwid.h"

LogComWid::LogComWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogComWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mSqlTableWid = new SqlTableWid(ui->widget);
    mDateWidget = new LOG_DateWidget(ui->dateWid);
    connect(mDateWidget,SIGNAL(selectedSig(const QString &)), mSqlTableWid,SIGNAL(querySig(const QString &)));
    connect(mSqlTableWid,SIGNAL(refreshSig()), mDateWidget,SLOT(updateSlot()));
}

LogComWid::~LogComWid()
{
    delete ui;
}

void LogComWid::initWid(BasicSql *db, LogBtnBar *bar, SqlQueryBase *btn)
{
    mDateWidget->setTable(db->tableName());

    // LogBtnBar *bar = new LogBtnBar;
    bar->setDlg(btn);
    mSqlTableWid->initWid(db, bar);
}


