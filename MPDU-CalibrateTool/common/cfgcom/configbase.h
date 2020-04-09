#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"


class ConfigBase
{
    ConfigBase();
public:
    static ConfigBase *bulid();

    QString getSerialName();
    void setSerialName(const QString &name);


    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
