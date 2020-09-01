#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"


class ConfigBase
{
public:
    ConfigBase();
    static ConfigBase *bulid();

    QString getSerialName(const QString &com="");
    void setSerialName(const QString &name, const QString &com="");

    double getValue(const QString &name, const QString &v);
    void setValue(double value, const QString &name, const QString &v);

    QString getCurrentName();
    void setCurrentName(const QString &name);

    QString getSerialBr(const QString &com);
    void setSerialBr(const QString &com, const QString &br);


    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
