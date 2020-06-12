#ifndef SN_MANAGETHREAD_H
#define SN_MANAGETHREAD_H

#include "col_mpduthread.h"

struct sSnItem
{
    sSnItem() {}
    char devType[2];
    uchar date[3];
    ushort num;
    uchar pc;
    uchar exor;
    QString sn;
};

class SN_ManageThread : public QThread
{
    Q_OBJECT
    explicit SN_ManageThread(QObject *parent = nullptr);
public:
    static SN_ManageThread *bulid(QObject *parent = nullptr);

    bool snEnter();

protected:
    QString toSnStr(sSnItem &it);
    bool analySn(uchar *sn, int len, sSnItem &it);
    bool checkSn(uchar *sn, int len);
    void initReadCmd(sRtuItem &item);

    void createSn(sSnItem &it);
    int toSnData(sSnItem &it, uchar *data);
    void initWriteCmd(sRtuSetItem &item, uchar *data, int len);

    bool readSn(sSnItem &itSn);
    bool writeSn(sSnItem &itSn);
    void writeStatus(bool ret);

private:
    sSnItem mSnItem;
    Ad_Modbus *mModbus;
    sConfigItem *mItem;
    sDataPacket *mPacket;
};

#endif // SN_MANAGETHREAD_H
