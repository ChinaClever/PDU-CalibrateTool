#ifndef DATATABLEWID_H
#define DATATABLEWID_H

#include "comtablewid.h"
#include "col_mpduthread.h"

class Home_DataTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Home_DataTabWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void appendItem(sDataUnit *unit);

signals:

protected slots:
    void timeoutDone();

private:
    sDataUnit *mData;
    sConfigItem *mItem;
};

#endif // DATATABLEWID_H
