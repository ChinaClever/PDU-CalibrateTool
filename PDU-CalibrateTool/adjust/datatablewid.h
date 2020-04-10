#ifndef DATATABLEWID_H
#define DATATABLEWID_H

#include "comtablewid.h"
#include "adjustcorethread.h"

class DataTableWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit DataTableWid(QWidget *parent = nullptr);

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
