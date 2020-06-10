#ifndef LANDINGUSER_H
#define LANDINGUSER_H
#include "dbuserlog.h"

QString user_land_name();

class LandingUser : public QObject
{
    Q_OBJECT
    LandingUser(QObject *parent = 0);
public:
    static LandingUser* get(QObject *parent = 0);
    sUserItem user;
    bool land;

    void sig() {emit landSig();}

signals:
    void landSig();

protected:
    void init();

};

#endif // LANDINGUSER_H
