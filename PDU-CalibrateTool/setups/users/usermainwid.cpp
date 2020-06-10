/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "usermainwid.h"
#include "ui_usermainwid.h"

UserMainWid::UserMainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMainWid)
{
    ui->setupUi(this);

    LandingUser::get();
    mBtnBar = new UserBtnBar;
    mDbTable = new SqlTableWid(ui->widget);
    mDbTable->initWid(DbUser::bulid(), mBtnBar);
}

UserMainWid::~UserMainWid()
{
    delete ui;
}
