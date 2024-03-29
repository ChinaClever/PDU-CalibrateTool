#ifndef SN_MANAGETHREAD_H
#define SN_MANAGETHREAD_H

#include "col_rpduthread.h"

struct sSnItem
{
    sSnItem() {}
    uchar devType[4];
    uchar date[4];
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
    void toSnStr(sSnItem &it);
    bool analySn(uchar *sn, int len, sSnItem &it);
    bool checkSn(uchar *sn, int len);
    void initReadCmd(sRtuItem &item);
    void initDevType(sSnItem &it);

    void createSn(sSnItem &it);
    int toSnData(sSnItem &it, uchar *data);
    void initWriteCmd(sRtuSetItems &item, uchar *data, int len);

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
