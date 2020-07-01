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
    int readScreenVal(float targetCur);
    bool readScreenStableVal(float targetCur, int time=10);

protected:
    int setRange();
    int write(QByteArray &array);
    int read(QByteArray &witeArray, QByteArray &readArray, int msecs);
    bool delay(int s);

protected slots:
    void initSerialSlot();

private:
    SerialPort *mSerial;
};

#endif // STANDARDSOURCE_H
