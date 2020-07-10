#ifndef STANDARDSOURCE_H
#define STANDARDSOURCE_H

#include "ad_config.h"

class YC_StandSource : public QThread
{
    Q_OBJECT
    explicit YC_StandSource(QObject *parent = nullptr);
public:
    static YC_StandSource *bulid(QObject *parent = nullptr);

    bool powerOn(int v=60);  // 上电
    void powerDown(); // 下电
    bool powerReset(); //
    bool setCur(int v);

protected:
    bool setRange();
    bool delay(int s);
    bool write(QByteArray &array);
    bool setValue(const QString &str, int v);

protected slots:
    void initSerialSlot();

private:
    SerialPort *mSerial;
};

#endif // STANDARDSOURCE_H
