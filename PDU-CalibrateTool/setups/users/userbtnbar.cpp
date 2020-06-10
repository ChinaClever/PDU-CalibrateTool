/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "userbtnbar.h"

UserBtnBar::UserBtnBar(QWidget *parent) : SqlBtnBar(parent)
{
    clearHidden();
    queryHidden();
    importHidden();
}

void UserBtnBar::addBtn()
{
    NewUsrDlg dlg(this);
    dlg.exec();
}


void UserBtnBar::modifyBtn(int id)
{
    EditUsrDlg dlg(this);
    dlg.setUsrId(id);
    dlg.exec();
}

bool UserBtnBar::delBtn(int id)
{
    bool ret = false;

    DbUser* db = DbUser::bulid();
    int rtn = db->counts();
    if(rtn > 1) {
        QString name = LandingUser::get()->user.name;
        sUserItem user = db->findById(id);
        if(name != user.name) {
            ret = true;
        } else {
            InfoMsgBox box(this,tr("无法删除自己！！！"));
        }
    } else {
        InfoMsgBox box(this,tr("仅有一个账户无法删除！！"));
    }

    return ret;
}
