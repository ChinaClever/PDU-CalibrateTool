/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "home_workwid.h"
#include "ui_home_workwid.h"
#include "home_debugdlg.h"
extern bool usr_land_jur();

Home_WorkWid::Home_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_WorkWid)
{
    ui->setupUi(this);
    mSetDlg = new Home_SetDlg(this);
    mItem = Ad_Config::bulid()->item;
    mCore = Ad_CoreThread::bulid(this);
    ui->addrSpin->setValue(mItem->addr);

    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 10, 0, 0);
    gridLayout->addWidget(this);
}

Home_WorkWid::~Home_WorkWid()
{
    delete ui;
}

bool Home_WorkWid::initWid()
{
    bool ret = mItem->serial->isOpened();
    if(!ret){CriticalMsgBox box(this, tr("请先打开串口!")); return ret;}

    mItem->startTime = QTime::currentTime();
    mItem->addr = ui->addrSpin->value();
    Ad_Config::bulid()->setAddr(mItem->addr);

    ui->devTypeLab->setText("---");
    ui->snLab->setText(tr("%1 ---").arg(mItem->user));
    ui->statusLab->setText("---");

    QString str = tr("设备数量:%1  成功:%2  失败:%3")
            .arg(mItem->cnt.all).arg(mItem->cnt.ok).arg(mItem->cnt.err);
    ui->tgLab->setText(str);

    return ret;
}

void Home_WorkWid::on_startBtn_clicked()
{
    bool en = false;
    QString str = tr("停止校准");
    if(mItem->step == Test_Over) {
        bool ret = initWid();
        if(ret){
            mCore->startAdjust();
            emit startSig();
        } else {
            return;
        }
    } else {
        QuMsgBox box(this, tr("是否停止校准?"));
        if(box.Exec()) {
            en = true;
            str = tr("开始校准");
            mItem->step = Test_Over;
            ui->timeLab->setText(tr("已停止"));
        } else {
            return;
        }
    }

    ui->startBtn->setText(str);
    ui->setBtn->setEnabled(en);
    ui->addrSpin->setEnabled(en);
    ui->groupBox_2->setEnabled(en);
}

void Home_WorkWid::upTgWid()
{
    QString str = tr("设备数量:%1 成功:%2 失败:%3 ").arg(mItem->cnt.all).arg(mItem->cnt.ok).arg(mItem->cnt.err);
    sDataPacket *packet = sDataPacket::bulid();
    sTgObjData *tg = packet->tg;
    if(tg->cur > 0) {
        float curRate = packet->data->rate * COM_RATE_CUR;
        float powRate = packet->data->rate * COM_RATE_POW;
        if(curRate!=0&&powRate!=0)
            str += tr("总电流：%1A  总功率：%2Kw").arg(tg->cur/curRate).arg(tg->pow/powRate);
    }

    ui->tgLab->setText(str);
}

void Home_WorkWid::endFun()
{
    mItem->step = Test_Over;
    ui->groupBox_2->setEnabled(true);
    ui->startBtn->setText(tr("开始校准"));
    ui->startBtn->setEnabled(true);
    ui->addrSpin->setEnabled(true);
    ui->setBtn->setEnabled(true);

    ui->updateBtn->setText(tr("开始采集"));
    ui->updateBtn->setEnabled(true);

    ui->resBtn->setText(tr("开始较验"));
    ui->resBtn->setEnabled(true);    
}

void Home_WorkWid::upStatusLab()
{
    sDataPacket *packet = sDataPacket::bulid();
    QString str = packet->status;
    ui->statusLab->setText(str);

    ui->devTypeLab->setText(packet->dev_type);
    str = tr("%1 %2").arg(mItem->user).arg(packet->sn);
    ui->snLab->setText(str);

    QPalette pe;
    switch (packet->pass) {
    case 0:  pe.setColor(QPalette::WindowText, Qt::black); break;
    case 1:  pe.setColor(QPalette::WindowText, Qt::darkGreen); break;
    case 2:  pe.setColor(QPalette::WindowText, Qt::red); break;
    }
    ui->statusLab->setPalette(pe);
}

QString Home_WorkWid::getTime()
{
    QTime t(0,0,0,0);
    t = t.addSecs(mItem->startTime.secsTo(QTime::currentTime()));
    return  tr("%1").arg(t.toString("mm:ss"));
}

void Home_WorkWid::upLabColor()
{
    QString str = tr("---");
    sDataPacket *packet = sDataPacket::bulid();
    QString strColor;

    if(mItem->step) {
        str = getTime();
        strColor = "background-color:yellow; color:rgb(0, 0, 0);";
    } else {
        switch (packet->pass) {
        case Test_Success:
            str = tr("成功");
            strColor = "background-color:green; color:rgb(255, 255, 255);";
            break;
        case Test_Fail:
            str = tr("失败");
            strColor = "background-color:red; color:rgb(255, 255, 255);";
            break;
        }
    }

    if(strColor.size()) {
        strColor += "font:100 34pt \"微软雅黑\";";
    }

    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(strColor);
}

void Home_WorkWid::timeoutDone()
{
    upTgWid();
    upLabColor();
    upStatusLab();

    if(mItem->step == Test_End) {
        endFun();
    }

    //if(usr_land_jur()) ui->deBtn->show(); else ui->deBtn->hide();
}


void Home_WorkWid::on_resBtn_clicked()
{
    bool en = false;
    QString str = tr("停止较验");
    if(!initWid()) return;

    if(mItem->step != Test_vert) {
        mCore->startResult();
    } else {
        en = true;
        str = tr("开始较验");
        mItem->step = Test_Over;
    }

    ui->resBtn->setText(str);
    ui->setBtn->setEnabled(en);
    ui->startBtn->setEnabled(en);
    ui->updateBtn->setEnabled(en);
}

void Home_WorkWid::on_deBtn_clicked()
{
    if(!usr_land_jur()) {
        CriticalMsgBox box(this, tr("您没有权限进行此操作"));
        return;
    }

    if(mItem->step != Collect_Start) { // 开始采集数据
        on_updateBtn_clicked();
    }

    Home_DebugDlg dlg(this);
    dlg.exec();

    on_updateBtn_clicked(); // 停止采集数据
}

void Home_WorkWid::on_updateBtn_clicked()
{
    bool en = false;
    QString str = tr("停止采集");
    if(!initWid()) return;

    if(mItem->step != Collect_Start) {
        mCore->startCollect();
    } else {
        en = true;
        str = tr("开始采集");
        mItem->step = Test_Over;
    }

    ui->updateBtn->setText(str);
    ui->setBtn->setEnabled(en);
    ui->resBtn->setEnabled(en);
    ui->startBtn->setEnabled(en);
}

void Home_WorkWid::on_setBtn_clicked()
{
    mSetDlg->exec();
}
