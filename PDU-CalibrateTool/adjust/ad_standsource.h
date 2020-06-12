#ifndef STANDARDSOURCE_H
#define STANDARDSOURCE_H

#include "ad_config.h"

class Ad_StandSource : public QThread
{
    Q_OBJECT
    explicit Ad_StandSource(QObject *parent = nullptr);
public:
    static Ad_StandSource *bulid(QObject *parent = nullptr);

    int powerOn(int v=60);  // 上电
    int powerDown(); // 下电
    void powerReset(); //
    void readScreenVal();

protected:
    int setRange();
    int write(QByteArray &array);
    int read(QByteArray &witeArray, QByteArray &readArray, int msecs);

signals:

private:
    SerialPort *mSerial;
};

#endif // STANDARDSOURCE_H
