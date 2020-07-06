#ifndef STANDARDSOURCE_H
#define STANDARDSOURCE_H

#include "ad_config.h"

class YC_StandSource : public QThread
{
    Q_OBJECT
    explicit YC_StandSource(QObject *parent = nullptr);
public:
    static YC_StandSource *bulid(QObject *parent = nullptr);

    int powerOn(int v=60);  // 上电
    int powerDown(); // 下电
    void powerReset(); //

protected:
    int setRange();
    bool delay(int s);
    int write(QByteArray &array);
    int read(QByteArray &witeArray, QByteArray &readArray, int msecs);

#if 0
    int readScreenVal(float targetCur);
    bool readScreenStableVal(int targetCur, int time=10);
#endif

protected slots:
    void initSerialSlot();

private:
    SerialPort *mSerial;
};

#endif // STANDARDSOURCE_H
