#ifndef SYSCONFIGFILE_H
#define SYSCONFIGFILE_H
#include <QtCore>

bool com_cfg_open(void);
void com_cfg_close(void);
QString com_cfg_readStr(const QString &str, const QString &strGroup="System");
int com_cfg_readInt(const QString &, const QString &strGroup="System");
double com_cfg_readDouble(const QString &, const QString &strGroup="System");
void com_cfg_write(const QString &str, const QString &, const QString &strGroup="System");

void com_cfg_writeParam(const QString &, const QString &, const QString &strGroup="System");
extern QString cm_pathOfData(const QString& name);

#endif // SYSCONFIGFILE_H

