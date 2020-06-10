#include "navbarwid.h"
#include "ui_navbarwid.h"
#include "backcolourcom.h"

NavBarWid::NavBarWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavBarWid)
{
    ui->setupUi(this);
    set_background_icon(this,":/image/title_back.jpg");

}

NavBarWid::~NavBarWid()
{
    delete ui;
}

void NavBarWid::on_homeBtn_clicked()
{
    emit navBarSig(0);
}



void NavBarWid::on_setBtn_clicked()
{
    emit navBarSig(1);
}

void NavBarWid::on_logBtn_clicked()
{
    emit navBarSig(2);
}


void NavBarWid::on_loginBtn_clicked()
{

}
