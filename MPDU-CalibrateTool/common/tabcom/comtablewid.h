#ifndef COMTABLEWID_H
#define COMTABLEWID_H

#include <QtCore>
#include <QWidget>
#include <QGridLayout>
#include "backcolourcom.h"
#include <QTableWidgetItem>

namespace Ui {
class ComTableWid;
}

class ComTableWid : public QWidget
{
    Q_OBJECT

public:
    explicit ComTableWid(QWidget *parent = 0);
    ~ComTableWid();

    int getList(QList<QStringList> list);
protected:
    void initTableWid(QStringList &header, int line, const QString &title);
    void setTableItem(int id, int column, const QString &str);
    void setTableRow(int id, QStringList &listStr);
    void setTableRow(int id, int col, QStringList &listStr);
    void appendTableRow(QStringList &listStr, bool c=false);
    void checkTableRow(int line);

    void clearRow(int row);
    void clearTable();
    void delTable();
    void setBackgroundColor(int id);
    void setItemColor(int id, int column, int alarm);
    void scrollToBottomTable();

    int currentRow();
    int currentColumn();

public slots:
    virtual void timeoutDone(){}
    virtual void itemDoubleSlot(){}
    virtual void itemDoubleClicked(QTableWidgetItem *);

protected:
    QTimer *timer;

private:
    void initTableWidget(QStringList &header);
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &list);
    void addInitRow();
    void addTableRows(int line);
    void delTableRows(int line);

private:
    Ui::ComTableWid *ui;
    QStringList mHeader;
};

#endif // COMTABLEWID_H
