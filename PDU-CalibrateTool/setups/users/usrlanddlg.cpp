/*
 * usrlanddlg.cpp
 *  登录
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrlanddlg.h"
#include "ui_usrlanddlg.h"
#include <QApplication>

UsrLandDlg::UsrLandDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsrLandDlg)
{

    ui->setupUi(this);
    //    this->setWindowTitle(tr("用户登录"));
    //    setWindowIcon(QIcon(":/images/logo.jpg"));

    QPixmap map1(":/image/usr_land.jpg");
    QPixmap map=map1.scaled(800,600);
    QPalette palette;
    // palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/usr_land.jpg")));
    palette.setBrush(QPalette::Background, QBrush(map));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->landBtn->setFlat(true);
    QFont font=ui->label_5->font();
    font.setLetterSpacing(QFont::AbsoluteSpacing,100);
}

UsrLandDlg::~UsrLandDlg()
{

    delete ui;
}


/**
 * @brief 选择工作模式
 * @return
 */
int UsrLandDlg::selectWork(void)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("操作选择"));
    //    com_setBackColour(tr("信息提示"),&msgBox);
    msgBox.setText(tr("信息提示"));
    msgBox.setText(tr("\n您是想切换用户，还是想退出？\n"));
    msgBox.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel | QMessageBox::Open);
    msgBox.setButtonText (QMessageBox::Ok,QString(tr("切换用户")));
    msgBox.setButtonText (QMessageBox::Open,QString(tr("退出登录")));
    msgBox.setButtonText (QMessageBox::Cancel,QString(tr("取 消")));
    msgBox.setDefaultButton(QMessageBox::Ok);
    int res = msgBox.exec();
    switch (res) {
    case QMessageBox::Ok:
        res = 1; /*切换用户*/
        break;
    case QMessageBox::Open:
        res = 2; /*退出登录*/
        break;
    case QMessageBox::Cancel:
        res = 0;
        break;
    default:
        res = 0;
        break;
    }

    return res;
}

/**
 * @brief 用户退出
 * @return
 */
bool UsrLandDlg::quitWidget(void)
{
    QString name = LandingUser::get()->user.name;
    QuMsgBox box(0,tr("是否退出当前用户:%1").arg(name));
    bool ret = box.Exec();
    if(ret) {
        sUserLogItem item;
        item.name = name;
        item.remarks = tr("用户退出");
        DbUserLog::bulid()->insertItem(item);
        LandingUser::get()->user.jur = false;
        LandingUser::get()->land = false;
        LandingUser::get()->sig();
    }

    return ret;
}

/**
 * @brief 用户登录验证
 */
void UsrLandDlg::usrLand()
{
    QString str;
    QString name = ui->nameLineEdit->text();
    if(!name.isEmpty())
    {
        DbUser* db = DbUser::bulid();
        int rtn = db->contains(name);
        if(rtn>0)
        {
            QString pwd = ui->pwdLineEdit->text();
            if(!pwd.isEmpty())
            {
                sUserItem user = db->selItemsByName(name);
                if(pwd == user.pwd)
                {
                    LandingUser::get()->land = true;
                    LandingUser::get()->user = user;
                    LandingUser::get()->sig();

                    sUserLogItem item;
                    item.name = name;
                    item.remarks = tr("用户登陆");
                    DbUserLog::bulid()->insertItem(item);

                    this->accept();
                    emit sendUserNameSig(name);
                }
                else
                    str = tr("密码错误");
            }
            else
                str = tr("密码不能为空");
        }
        else
            str = tr("账号有误");
    }
    else
        str = tr("账号不能为空");

    ui->stateLabel->setText(str);

}

/**
 * @brief 登录响应函数
 */
void UsrLandDlg::on_landBtn_clicked()
{
    usrLand();
}

void UsrLandDlg::on_pushButton_clicked()
{
    InfoMsgBox box(0,tr("程序即将退出！"));
    exit(0);
}


