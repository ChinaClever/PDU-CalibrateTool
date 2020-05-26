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

    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
