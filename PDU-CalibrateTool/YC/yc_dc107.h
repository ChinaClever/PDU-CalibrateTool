#ifndef YC_DC107_H
#define YC_DC107_H

#include "yc_ac92b.h"

class YC_Dc107 : public YC_StandSource
{
    Q_OBJECT
    explicit YC_Dc107(QObject *parent = nullptr);
public:
    static YC_Dc107 *bulid(QObject *parent = nullptr);

    bool handShake();
    void powerDown();
    bool powerOn(int v=60);

    bool setCur(int v, int sec=5);
    bool setVol(int v, int sec=5);

protected:
    bool setRange();
    bool sendCmd(const QByteArray &cmd);
    bool setValue(const char &c, char v1, char v2=0);
    bool serialWrite(const QByteArray &array);
};

#endif // YC_DC107_H
