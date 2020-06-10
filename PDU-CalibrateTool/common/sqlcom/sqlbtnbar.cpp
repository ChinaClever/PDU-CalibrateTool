/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "sqlbtnbar.h"
#include "ui_sqlbtnbar.h"
extern bool usr_land_jur();

SqlBtnBar::SqlBtnBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlBtnBar)
{
    ui->setupUi(this);
}

SqlBtnBar::~SqlBtnBar()
{
    delete ui;
}

void SqlBtnBar::importHidden()
{
    ui->importBtn->setHidden(true);
}

void SqlBtnBar::clearHidden()
{
    ui->clearBtn->setHidden(true);
}

void SqlBtnBar::queryHidden()
{
    ui->queryBtn->setHidden(true);
}

void SqlBtnBar::gridLayout(QWidget *parent)
{
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
    this->setParent(parent);
}

void SqlBtnBar::setNoEdit()
{
    ui->addBtn->setHidden(true);
    ui->modifyBtn->setHidden(true);
    ui->delBtn->setHidden(true);
    ui->importBtn->setHidden(true);
}

void SqlBtnBar::setEdit()
{
    ui->addBtn->setVisible(true);
    ui->modifyBtn->setVisible(true);
    ui->delBtn->setVisible(true);
    ui->importBtn->setVisible(true);
}

void SqlBtnBar::on_refreshBtn_clicked()
{
    emit refreshSig();
    InfoMsgBox box(this, tr("刷新成功!"));
}

bool SqlBtnBar::checkJur()
{
    bool ok, ret = false;
    QString text = QInputDialog::getText(this,tr("口令验证"),tr("请输入管理员密码"), QLineEdit::Normal,NULL,&ok);
    if(ok && !text.isEmpty()) {
        if(text == "123456") ret = true;
    }

    if(!ret) {
        InfoMsgBox box(this, tr("您无权操作此功能！"));
    }
    return ret;
}

void SqlBtnBar::on_addBtn_clicked()
{
    bool ret = checkJur();
    if(ret) {
        addBtn();
        emit refreshSig();
    }
}

void SqlBtnBar::on_modifyBtn_clicked()
{
    bool ret = checkJur();
    if(ret) {
        emit modifySig();
    }
}

void SqlBtnBar::modifySlot(int id)
{
    QString str = tr("请确认需要进行修改?");
    QuMsgBox box(this, str);
    bool ret = box.Exec();
    if(ret) {
        modifyBtn(id);
    }
    emit refreshSig();
}

void SqlBtnBar::delSlot(int id)
{
    bool ret = delBtn(id);
    if(!ret) return;

    QString str = tr("是否删除这条纪录?");
    QuMsgBox box(this, str);
    ret = box.Exec();
    if(ret) {
        emit delSig(id);
    }
}

void SqlBtnBar::on_delBtn_clicked()
{
    bool ret = checkJur();
    if(ret) {
        emit delSig();
    }
}

void SqlBtnBar::on_clearBtn_clicked()
{
    if(!checkJur()) return;
    QString str = tr("是否清空纪录?");
    QuMsgBox box(this, str);
    bool ret = box.Exec();
    if(ret) {
        emit clearSig();
        QCoreApplication::processEvents(QEventLoop::AllEvents,2250);
        emit refreshSig();
    }
}

void SqlBtnBar::on_queryBtn_clicked()
{
    QString str = queryBtn();
    if(!str.isEmpty()) {
        emit querySig(str);
    }
}

void SqlBtnBar::on_exportBtn_clicked()
{
    bool ret = checkJur();
    if(ret) {
        emit exportSig();
    }
}

void SqlBtnBar::on_importBtn_clicked()
{
    bool ret = checkJur();
    if(ret) {
        ret = importBtn();
        if(ret) {
            InfoMsgBox box(this, tr("数据导入成功!"));
            emit refreshSig();
        }
    }
}
