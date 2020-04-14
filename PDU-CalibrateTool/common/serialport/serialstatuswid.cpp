#include "serialstatuswid.h"
#include "ui_serialstatuswid.h"
#include "adjustconfig.h"

SerialStatusWid::SerialStatusWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialStatusWid)
{
    ui->setupUi(this);
    initSerialPort();
    mEnable = false;
}

SerialStatusWid::~SerialStatusWid()
{
    delete ui;
}


bool SerialStatusWid::checkSerial()
{
    SerialPort *serial = mSerialDlg->getSerialPort();
    bool ret = serial->isOpened();
    if(!ret) {
        QuMsgBox box(this, tr("请先打开串口!!"));
        ret = box.Exec();
        if(ret) {
            on_comBtn_clicked();
        }
    }
    //mEnable = serial->isOpened();

    return ret;
}

void SerialStatusWid::on_comBtn_clicked()
{
    if(mEnable != true) {
        mSerialDlg->exec();
        updateSerialWid();
    } else {
        CriticalMsgBox box(this, tr("请先停止！！！"));
    }
}


/**
 * @brief 初始化串口
 * @return
 */
bool SerialStatusWid::initSerialPort()
{
    bool ret = false;
    mSerialDlg = new SerialPortDlg(this);
    sConfigItem *item = AdjustConfig::bulid()->item;
    SerialPort *serial = item->serial = getSerialPort();
    QString com = AdjustConfig::bulid()->getSerialName();
    if(!com.isEmpty())
    {
        ret = serial->isContains(com);
        if(ret) {
            ret = serial->open(com);
            updateSerialWid();
        }
    }

    return ret;
}



/**
 * @brief 更新串口窗口
 */
void SerialStatusWid::updateSerialWid()
{
    QPalette pe;
    SerialPort *serial = mSerialDlg->getSerialPort();
    QString str = serial->getSerialName();
    if(serial->isOpened()) {
        AdjustConfig::bulid()->setSerialName(str);
        str += tr(" 已打开");
        pe.setColor(QPalette::WindowText,Qt::black);
    } else {
        str += tr(" 未打开");
        pe.setColor(QPalette::WindowText,Qt::red);
    }

    ui->serialLab->setText(str);
    ui->serialLab->setPalette(pe);
}

