#include "modeselectwid.h"
#include "ui_modeselectwid.h"

ModeSelectWid::ModeSelectWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeSelectWid)
{
    ui->setupUi(this);
    mData = sDataPacket::bulid()->data;
    mItem = AdjustConfig::bulid()->item;

    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
}

ModeSelectWid::~ModeSelectWid()
{
    delete ui;
}


void ModeSelectWid::initData()
{
    mItem->devType = ui->devTypeBox->currentIndex();
    mItem->addr = ui->addrBox->currentIndex()+1;
    mItem->mode = ui->modeBox->currentIndex();
    mItem->vol = ui->volBox->value() * 10;
    mItem->cur = ui->curBox->value() * 10;
    mItem->volErr = ui->volErrBox->value() * 10;
    mItem->curErr = ui->curErrBox->value() * 10;
    if(ui->allRadio->isChecked()) {
        mItem->vert = 1;
    } else {
        mItem->vert = 0;
    }

    mItem->step = Test_Start;
    sDataPacket::bulid()->clear();
    if(mItem->devType) mData->rate = 10; else mData->rate = 1;
    AdjustCoreThread::bulid(this)->start();
}

void ModeSelectWid::on_startBtn_clicked()
{
    bool en = false;
    QString str = tr("停止校准");
    if(mItem->step != Test_Start) {
        initData();
    } else {
        en = true;
        str = tr("开始校准");
        mItem->step = Test_Over;
    }

    ui->startBtn->setText(str);
    ui->groupBox_1->setEnabled(en);
    ui->groupBox_3->setEnabled(en);
    if(!ui->modeBox->currentIndex())
        ui->groupBox_2->setEnabled(en);
}

void ModeSelectWid::on_modeBox_currentIndexChanged(int index)
{
    if(index) {
        ui->oneRradio->setChecked(true);
        ui->groupBox_2->setEnabled(false);
    } else {
        ui->allRadio->setChecked(true);
        ui->groupBox_2->setEnabled(true);
    }
}


void ModeSelectWid::timeoutDone()
{
    sDataPacket *packet = sDataPacket::bulid();
    QString str = packet->status;
    ui->statusLab->setText(str);

    QPalette pe;
    switch (packet->pass) {
    case 0:  pe.setColor(QPalette::WindowText, Qt::black); break;
    case 1:  pe.setColor(QPalette::WindowText, Qt::green); break;
    case 2:  pe.setColor(QPalette::WindowText, Qt::red); break;
    }
    ui->statusLab->setPalette(pe);

    if(mItem->step == Test_End) {
        mItem->step = Test_Start;
        on_startBtn_clicked();
    }
}
