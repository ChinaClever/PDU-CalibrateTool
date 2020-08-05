#ifndef YC_AC92B_H
#define YC_AC92B_H

#include "yc_standsource.h"

class YC_Ac92b : public YC_StandSource
{
    Q_OBJECT
    explicit YC_Ac92b(QObject *parent = nullptr);
public:
    static YC_Ac92b *bulid(QObject *parent = nullptr);

    bool handShake();
    void powerDown();
    bool powerOn(int v=60);

    bool setCur(int v);
    bool setVol();

protected:
    bool initFun();
    bool setRange();
    bool setValue(const QString &str, int v);
    bool serialWrite(QByteArray &array);
};

#endif // YC_AC92B_H
