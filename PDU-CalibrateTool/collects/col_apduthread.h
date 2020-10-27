#ifndef COL_APDUTHREAD_H
#define COL_APDUTHREAD_H
#include "col_mpduthread.h"

class Col_ApduThread: public Col_CoreThread
{
    Q_OBJECT
    explicit Col_ApduThread(QObject *parent = nullptr);
public:
    static Col_ApduThread *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvApduVolCur(uchar *recv, int len);
    bool getApduVolCur();
};

#endif // COL_APDUTHREAD_H
