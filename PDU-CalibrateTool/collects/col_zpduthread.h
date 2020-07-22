#ifndef COL_ZPDUTHREAD_H
#define COL_ZPDUTHREAD_H

#include "col_corethread.h"

class Col_ZpduThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_ZpduThread(QObject *parent = nullptr);
public:
    static Col_ZpduThread *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvZpduVolCur(uchar *recv, int len);
    bool getZpduVolCur();
};

#endif // COL_ZPDUTHREAD_H
