#ifndef STANDARDSOURCE_H
#define STANDARDSOURCE_H

#include "adjustconfig.h"

class StandardSource : public QThread
{
    Q_OBJECT
    explicit StandardSource(QObject *parent = nullptr);
public:
    static StandardSource *bulid(QObject *parent = nullptr);

    void powerOn();  // 上电
    void powerDown(); // 下电
    void powerReset(); //

protected:
    void setRange();
    int write(QByteArray &array);

signals:

private:
    SerialPort *mSerial;
};

#endif // STANDARDSOURCE_H
