/*
 * newusrdlg.cpp
 *  用户增加界面
 *  新用户信息增加
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "newusrdlg.h"
#include "ui_newusrdlg.h"

NewUsrDlg::NewUsrDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUsrDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);

    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->TelephonelineEdit);
    ui->TelephonelineEdit->setValidator( validator );
}

NewUsrDlg::~NewUsrDlg()
{
    delete ui;
}

/**
 * @brief 修改标题
 * @param str
 */
void NewUsrDlg::editTitle(const QString &str)
{
    ui->NamelineEdit->setEnabled(false);
    ui->Titlelabel->setText(str);
    this->setWindowTitle(str);

    checkUsr();
}

/**
 * @brief 编辑时，如果只有一个账户，这个账户只能是管理员
 */
void NewUsrDlg::checkUsr(void)
{
    DbUser* db = DbUser::bulid();
    int ret = db->counts();
    if(ret <= 1) {
        ui->authcomboBox->setCurrentIndex(1);
        ui->authcomboBox->setDisabled(true);
    }
}


/**
 * @brief 用户信息加载
 * @param usr
 */
void NewUsrDlg::loadUsrInfo(sUserItem &user)
{
    ui->NamelineEdit->setText(user.name);
    ui->PwdlineEdit->setText(user.pwd);
    ui->pwd2lineEdit->setText(user.pwd);
    ui->authcomboBox->setCurrentIndex(user.jur);
    ui->TelephonelineEdit->setText(user.telephone);
    ui->EmaillineEdit->setText(user.email);
    ui->RemarkstextEdit->setText(user.remarks);
}


/**
 * @brief 输入信息检查
 * @return
 */
bool NewUsrDlg::informationCheck(void)
{
    QString str;

    str = ui->NamelineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL, tr("账号不能为空"));
        return false;
    }

    str = ui->PwdlineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL,  tr("密码不能为空"));
        return false;
    }

    if(str != ui->pwd2lineEdit->text())
    {
        InfoMsgBox(NULL,  tr("密码两次输入不一致"));
        return false;
    }

    str = ui->EmaillineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL,  tr("请输入Email地址"));
        return false;
    }
    //if(!str.contains("@"))
    if(!IsValidEmail(str))
    {
        InfoMsgBox(NULL,  tr("请输入合法Email地址"));
        return false;
    }

    str = ui->TelephonelineEdit->text();
    if(str.isEmpty())
    {
        InfoMsgBox(NULL, tr("请输入工号"));
        return false;
    }

   return true;
}

bool NewUsrDlg::IsValidChar(QChar ch)
{
    if( (ch>=97) && (ch<=122) ) //26个小写字母
        return true;
    if( (ch>=65) && (ch<=90) ) //26个大写字母
        return true;
    if((ch>=48) && (ch<=57)) //0~9
        return true;
    if( ch==95 || ch==45 || ch==46 || ch==64 ) //_-.@
        return true;
    return false;
}

bool NewUsrDlg::IsValidEmail(QString strEmail)
{
    if( strEmail.length()<5 ) //26个小写字母
        return false;

    QChar ch = strEmail[0];
    if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) )
    {
        int atCount =0;
        int atPos = 0;
        int dotCount = 0;
        for(int i=1;i<strEmail.length();i++) //0已经判断过了，从1开始
        {
            ch = strEmail[i];
            if(IsValidChar(ch))
            {
                if(ch==64) //"@"
                {
                    atCount ++;
                    atPos = i;
                }
                else if( (atCount>0) && (ch==46) )//@符号后的"."号
                    dotCount ++;
            }
            else
                return false;
        }
        //1. 结尾不得是字符“@”或者“.”
        if( ch == 46 )
            return false;
        //2. 必须包含一个并且只有一个符号“@”
        //3. @后必须包含至少一个和至多三个符号“.”
        if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
            return false;
        //4. 不允许出现“@.”或者.@
        int x,y;
        x=strEmail.indexOf("@.");
        y=strEmail.indexOf(".@");
        if( x>0 || y>0 )
        {
            return false;
        }
        return true;
    }
    return false;
}

bool NewUsrDlg::saveUsrInfo(sUserItem &user)
{
    DbUser* db = DbUser::bulid();
    int rtn = db->contains(user.name);
    if(rtn > 0) return false;

    return db->insertItem(user);
}

void NewUsrDlg::on_saveBtn_clicked()
{
    if(informationCheck())
    {
        sUserItem user;
        user.name = ui->NamelineEdit->text();
        user.pwd = ui->PwdlineEdit->text();
        user.jur = ui->authcomboBox->currentIndex();
        user.jurisdiction = ui->authcomboBox->currentText();
        user.telephone = ui->TelephonelineEdit->text();
        user.email = ui->EmaillineEdit->text();
        user.remarks = ui->RemarkstextEdit->toPlainText();

        if(saveUsrInfo(user))
            accept();
        else
            InfoMsgBox(NULL,  tr("账号已存在"));
    }
}
