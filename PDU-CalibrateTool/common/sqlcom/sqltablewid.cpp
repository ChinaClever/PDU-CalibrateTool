/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "sqltablewid.h"
#include "ui_sqltablewid.h"

SqlTableWid::SqlTableWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlTableWid)
{
    ui->setupUi(this);
    mExportDlg = new SqlExportDlg(this);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

SqlTableWid::~SqlTableWid()
{
    delete ui;
}


void SqlTableWid::initWid(BasicSql *db, SqlBtnBar *btn)
{
    mBtnBar = btn;
    mTableTile = db->tableTile;
    btn->gridLayout(ui->btnWid);
    btn->setParent(ui->btnWid);

    mTableView = new SqlTableView(ui->wid);
    mTableView->initTable(db);
    initFunSLot();
}



void SqlTableWid::initFunSLot()
{
    connect(mBtnBar,SIGNAL(exportSig()),this,SLOT(exportSlot()));
    connect(mBtnBar,SIGNAL(delSig()),mTableView,SLOT(delSlot()));
    connect(mTableView,SIGNAL(delSig(int)),mBtnBar,SLOT(delSlot(int)));
    connect(mBtnBar,SIGNAL(delSig(int)),mTableView,SLOT(delSlot(int)));
    connect(mBtnBar,SIGNAL(modifySig()),mTableView,SLOT(modifySlot()));
    connect(mTableView,SIGNAL(modifySig(int)),mBtnBar,SLOT(modifySlot(int)));
    connect(mBtnBar,SIGNAL(clearSig()),mTableView,SLOT(clearTableSlot()));
    connect(mBtnBar,SIGNAL(refreshSig()),mTableView,SLOT(refreshSlot()));
    connect(mBtnBar,SIGNAL(querySig(const QString &)),mTableView,SLOT(querySlot(const QString &)));
    connect(this,SIGNAL(querySig(const QString &)),mTableView,SLOT(querySlot(const QString &)));
    connect(mBtnBar,SIGNAL(refreshSig()),this,SIGNAL(refreshSig()));
}

void SqlTableWid::exportSlot()
{
    QList<QStringList> list;
    mTableView->getList(list);
    mExportDlg->init(mTableTile, list);
    mExportDlg->exec();
}
