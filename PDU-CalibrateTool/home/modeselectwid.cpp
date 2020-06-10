/*
 *
 *  Created on: 2019年10月1日
 *      Author: Lzy
 */
#include "modeselectwid.h"
#include "ui_modeselectwid.h"
#include "home_debugdlg.h"

ModeSelectWid::ModeSelectWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeSelectWid)
{
    ui->setupUi(this);
    initSerial();

    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;

    mRangeDlg = new Home_RangeDlg(this);
    mRangeDlg->initData(mItem);

    mMpduThread = AdjustMpduThread::bulid(this);
    mZpduThread = AdjustZpduThread::bulid(this);
    on_modeBox_currentIndexChanged(0);

    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
}

ModeSelectWid::~ModeSelectWid()
{
    delete ui;
}

void ModeSelectWid::initSerial()
{
    sConfigItem *item = AdjustConfig::bulid()->item;
    mSerialWid = new SerialStatusWid(ui->serialWid);
    item->serial = mSerialWid->initSerialPort(tr("校准源"), 19200);

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    item->source = mSourceWid->initSerialPort(tr("标准源"), 9600);
}



bool ModeSelectWid::initData()
{
    bool ret = mItem->serial->isOpened();
    if(!ret){CriticalMsgBox box(this, tr("请先打开串口!")); return ret;}

    mRangeDlg->initData(mItem);
    mItem->devType = ui->devTypeBox->currentIndex();
    mItem->addr = ui->addrBox->currentIndex()+1;
    mItem->adMode = ui->modeBox->currentIndex();

    if(mItem->devType) {
        mData->rate = 10.0;
    } else {
        mData->rate = 1.0;
    }

    QString str = tr("总电流：---    总功率：---  ");
    ui->resLab->setText(str);

    return ret;
}

void ModeSelectWid::setEnablWid(bool en)
{
    ui->groupBox_1->setEnabled(en);
    ui->groupBox_2->setEnabled(en);
}

void ModeSelectWid::on_startBtn_clicked()
{
    bool en = false;
    QString str = tr("停止校准");
    if(mItem->step != Test_Start) {
        bool ret = initData();
        if(ret){
            mItem->step = Test_Start;
            mCoreThread->start();
        } else {
            return;
        }
    } else {
        QuMsgBox box(this, tr("是否停止校准?"));
        if(box.Exec()) {
            en = true;
            str = tr("开始校准");
            mItem->step = Test_Over;
        } else {
            return;
        }
    }

    setEnablWid(en);
    ui->startBtn->setText(str);
}

void ModeSelectWid::on_modeBox_currentIndexChanged(int index)
{
    mRangeDlg->setModel(index);
    if(index) {
        mCoreThread = mZpduThread;
    } else {
        mCoreThread = mMpduThread;
    }
}

void ModeSelectWid::upTgWid()
{
    sDataPacket *packet = sDataPacket::bulid();
    sTgObjData *tg = packet->tg;
    QString str = tr("总电流：%2A    总功率：%3KW").arg(tg->cur).arg(tg->pow);
    ui->resLab->setText(str);
}

void ModeSelectWid::endFun()
{
    upTgWid();
    mItem->step = Test_Over;
    setEnablWid(true);
    ui->startBtn->setText(tr("开始校准"));
}

void ModeSelectWid::timeoutDone()
{
    sDataPacket *packet = sDataPacket::bulid();
    if(mItem->step) {
        QString str = packet->status;
        ui->statusLab->setText(str);
    }

    QPalette pe;
    switch (packet->pass) {
    case 0:  pe.setColor(QPalette::WindowText, Qt::black); break;
    case 1:  pe.setColor(QPalette::WindowText, Qt::green); break;
    case 2:  pe.setColor(QPalette::WindowText, Qt::red); break;
    }
    ui->statusLab->setPalette(pe);
    if(mItem->step == Test_End) {
        endFun();
    } else if(mItem->step == Collect_Start) {
        upTgWid();
    }
}

void ModeSelectWid::on_onBtn_clicked()
{
    QuMsgBox box(this, tr("是否需要校准上电?"));
    if(!box.Exec()) return;

    int ret = Ad_StandSource::bulid(this)->powerOn();
    if(ret <= 0) {
        CriticalMsgBox box(this, tr("标准源上电失败!"));
    }
}

void ModeSelectWid::on_downBtn_clicked()
{
    QuMsgBox box(this, tr("是否需要校准下电?"));
    if(!box.Exec()) return;

    //int ret = StandardSource::bulid(this)->powerDown();
    Ad_StandSource::bulid(this)->readScreenVal();
    //    if(ret <= 0) {
    //        CriticalMsgBox box(this, tr("标准源下电失败!"));
    //    }
}


void ModeSelectWid::on_errBtn_clicked()
{
    mRangeDlg->exec();
}

void ModeSelectWid::on_reBtn_clicked()
{
    bool en = false;
    QString str = tr("停止采集");
    if(!initData()) return;

    if(mItem->step != Collect_Start) {
        mItem->step = Collect_Start;
        mCoreThread->start();
    } else {
        en = true;
        str = tr("开始采集");
        mItem->step = Test_Over;
    }

    ui->reBtn->setText(str);
    ui->startBtn->setEnabled(en);
    ui->groupBox_1->setEnabled(en);
}

void ModeSelectWid::on_deBtn_clicked()
{
    // 开始采集数据
    if(mItem->step != Collect_Start) {
        on_reBtn_clicked();
    }

    Home_DebugDlg dlg(this);
    dlg.exec();

    on_reBtn_clicked(); // 停止采集数据
}