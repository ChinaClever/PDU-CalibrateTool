#ifndef COL_MCRPDUTHREAD_H
#define COL_MCRPDUTHREAD_H

#include "col_ipthread.h"


enum R_SetCmdEnum {
    R_RtuReg_LineCur = 0x20C,//7  0x20C
    R_RtuReg_LineCurSize = 0x02 + 1,
    R_RtuReg_LineVol = 0x20F,//8  0x20F
    R_RtuReg_LineVolSize = 0x02 + 1,

    R_RtuReg_LinePF = 0x600,//19
    R_RtuReg_LinePFSize = 0x2+1,
    R_RtuReg_LineEle = 0x610,//20
    R_RtuReg_LineEleSize = 0x5+1,//取两个

    R_RtuReg_DevType= 0x820,//23  1-4 单相的A-D系列 5-8 三相的A-D系列
    R_RtuReg_DevTypeSize = 0x1,

    R_RtuReg_CmdNum = 5,
};

class Col_McRpduThread : public Col_CoreThread
{
    Q_OBJECT
    explicit Col_McRpduThread(QObject *parent = nullptr);
public:
    static Col_McRpduThread *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *ptr, int len);

};

#endif // COL_MCRPDUTHREAD_H
