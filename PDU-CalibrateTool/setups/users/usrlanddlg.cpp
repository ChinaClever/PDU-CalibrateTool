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
#include "ad_config.h"

UsrLandDlg::UsrLandDlg(QString name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsrLandDlg)
{

    ui->setupUi(this);
    this->setWindowTitle(tr("用户登录"));
    groupBox_background_icon(this);
    ui->nameLineEdit->setText(name);
    ui->pwdLineEdit->setText("");
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
    QMessageBox msgBox(this);
    groupBox_background_icon(&msgBox);
    msgBox.setWindowTitle(tr("操作选择"));
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
        LandingUser::get()->user.jur = false;
        LandingUser::get()->land = false;
        LandingUser::get()->sig();
        emit sendUserNameSig(tr("未登陆"));
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
                    this->accept();
                    emit sendUserNameSig(name);
                    Ad_Config *con = Ad_Config::bulid();
                    con->setName(name);
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

void UsrLandDlg::closeEvent(QCloseEvent *event)
{
    QuMsgBox box(0,tr("确定关闭程序？"));
    if (box.Exec()) {   //允许关闭
        event->accept();
        exit(0);
    }  else  {  //拒绝关闭
        event->ignore();
    }
}

