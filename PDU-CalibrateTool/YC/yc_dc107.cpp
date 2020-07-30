#include "yc_dc107.h"

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

bool YC_Dc107::serialWrite(QByteArray &array)
{
    bool ret = false;
    QByteArray res;
    int rtn = mSerial->transmit(array, res, 3);
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
    cmd[1] = v2;

    QByteArray array(cmd, sizeof(cmd));
    return sendCmd(array);
}


bool YC_Dc107::setPowers()
{
    return setValue('P', 0x11);
}

bool YC_Dc107::setRange()
{
//    bool ret = setPowers();
//    if(ret) {
//        delay(3);
        bool ret = setValue('U', 7);
//        if(ret) {
//            delay(3);
//            ret = setValue('I', 12);
//        }
//    }

    return ret;
}


bool YC_Dc107::setVol()
{
    bool ret = setRange();
//    if(ret) {
//        delay(3);
//        bool ret = setValue('V', 0, 0x20);
//    }

    return ret;
}

bool YC_Dc107::setCur(int v)
{
    bool ret = setValue('A', 0, v);
    if(ret) {
        ret = delay(10);
    }
    return ret;
}


void YC_Dc107::powerDown()
{
    setValue('A', 0);
    setValue('V', 0);
}


bool YC_Dc107::powerOn(int v)
{
    bool ret = setValue('V', 2);
    if(ret) {
        // ret = setCur('A', v);
    }

    return ret;
}

bool YC_Dc107::handShake()
{
    bool ret = setBaudRate(4800);
    if(ret) {
//        ret = setPowers();
        ret = setVol();

    }
    return ret;
}
