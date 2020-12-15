/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "yc_dc107.h"
#include "common.h"

YC_Dc107::YC_Dc107(QObject *parent) : YC_StandSource(parent)
{

}

YC_Dc107 *YC_Dc107::bulid(QObject *parent)
{
    static YC_Dc107* sington = nullptr;
    if(sington == nullptr)
        sington = new YC_Dc107(parent);
    return sington;
}

bool YC_Dc107::serialWrite(const QByteArray &array)
{
    bool ret = false;
    QByteArray res;
    int rtn = mSerial->transmit(array, res, 4);
    if(rtn > 0)  ret = true;

    return ret;
}

bool YC_Dc107::sendCmd(const QByteArray &cmd)
{
    QByteArray array;
    array.insert(0, 0x1B);
    array.append(cmd);

    uchar sum = 0;
    for(int i=0; i<cmd.size(); ++i) sum += cmd.at(i);
    array.append(sum);

    return write(array);
}

bool YC_Dc107::setValue(const char &c, char v1, char v2)
{
    char cmd[] = {'P', 0x11, 0, 0, 0};
    cmd[0] = c;
    cmd[1] = v1;
    cmd[2] = v2;

    QByteArray array(cmd, sizeof(cmd));
    return sendCmd(array);
}

bool YC_Dc107::setRange()
{
    bool ret = setValue('P', 0x11);
    if(ret) {
        delay(2);
        ret = setValue('U', 7);
        if(ret) {
            delay(2);
            ret = setValue('I', 12); delay(2);
        }
    }

    return ret;
}

bool YC_Dc107::setVol(int v, int sec)
{
    char cmd220[] = {'V', 0, 0x73, 0x33, 0x40};
    char cmd200[] = {'V', 0, 0x66, 0x70, 0};

    char *cmd = cmd220;
    if(v == 200) cmd = cmd200;
    QByteArray array(cmd, sizeof(cmd220));
    bool ret = sendCmd(array);
    if(ret) ret = delay(sec);

    return ret;
}

bool YC_Dc107::setCur(int v, int sec)
{
    v /= 10; v <<= 4;
    bool ret = setValue('A', 0, v);
    if(ret) ret = delay(sec);

    return ret;
}

void YC_Dc107::powerDown()
{
    setValue('A', 0);
    setValue('V', 0);
}

bool YC_Dc107::powerOn(int v)
{
    bool ret = setVol(220);
    if(ret) {
         ret = setCur(v);
    }

    return ret;
}

bool YC_Dc107::handShake()
{
    bool ret = setBaudRate(4800);
    if(ret) {
        ret = setRange();
        if(ret) {
            acOrDc = 2;
        } else {
            acOrDc = 0;
        }
    }
    return ret;
}
