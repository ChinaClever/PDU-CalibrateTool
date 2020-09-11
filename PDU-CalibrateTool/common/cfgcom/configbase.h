#ifndef CONFIGBASH
#define CONFIGBASH
#include "cfgcom.h"

class ConfigBase
{
protected:
    ConfigBase();

public:
    static ConfigBase *bulid();

    QString getSerialName(const QString &key);
    void setSerialName(const QString &key, const QString &v);

    void write(const QString &key, const QVariant& v, const QString &g="Cfg");
    QVariant read(const QString &key, const QVariant &v = QVariant(), const QString &g="Cfg");

    QString getCurrentName();
    void setCurrentName(const QString &name);

    QString getSerialBr(const QString &com);
    void setSerialBr(const QString &com, const QString &br);

protected:
    CfgCom *mCfg;
};

#endif // CONFIGBASH
