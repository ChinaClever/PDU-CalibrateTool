#ifndef STANDARDSOURCE_H
#define STANDARDSOURCE_H

#include "adjustconfig.h"

class StandardSource : public QThread
{
    Q_OBJECT
    explicit StandardSource(QObject *parent = nullptr);
public:
    static StandardSource *bulid(QObject *parent = nullptr);

    int powerOn();  // 上电
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
