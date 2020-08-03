#include "navbarwid.h"
#include "ui_navbarwid.h"
#include "backcolourcom.h"
#include "readmedlg.h"
#include "ad_config.h"

NavBarWid::NavBarWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavBarWid)
{
    ui->setupUi(this);
    set_background_icon(this,":/image/title_back.jpg");
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 6);
    gridLayout->addWidget(this);

    Ad_Config *con = Ad_Config::bulid();
    QString name = con->initName();
    mUserLand = new UsrLandDlg(name,this);

    timer = new QTimer(this);
    //timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));

    QTimer::singleShot(5,this,SLOT(on_loginBtn_clicked()));
    connect(mUserLand,SIGNAL(sendUserNameSig(QString)),this,SLOT(recvUserNameSlot(QString)));
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
    bool lang = LandingUser::get()->land;
    if(lang) {
        int ret = mUserLand->selectWork();
        if(ret == 1) { // 用户切换
            mUserLand->exec();
        }  else if(ret == 2) { // 用户退出
            mUserLand->quitWidget();
        }
    } else {
        mUserLand->exec();
    }
}


void NavBarWid::recvUserNameSlot(QString str)
{
    ui->userLab->setText(str);
}

void NavBarWid::timeoutDone()
{
    if(usr_land_jur())
        ui->setBtn->show();
    else
        ui->setBtn->hide();
}

void NavBarWid::on_readmeBtn_clicked()
{
    ReadMeDLg dlg(this);
    dlg.exec();
}
