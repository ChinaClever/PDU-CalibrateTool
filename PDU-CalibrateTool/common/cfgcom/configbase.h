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

    double getValue(const QString &name);
    void setValue(double value, const QString &name);



    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
