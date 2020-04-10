#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"


class ConfigBase
{
public:
    ConfigBase();
    static ConfigBase *bulid();

    QString getSerialName();
    void setSerialName(const QString &name);


    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
