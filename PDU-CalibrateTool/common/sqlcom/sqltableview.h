#ifndef SQLTABLEVIEW_H
#define SQLTABLEVIEW_H
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QGridLayout>
#include "sqltablemodel.h"

class SqlTableView : public QWidget
{
    Q_OBJECT
public:
    explicit SqlTableView(QWidget *parent = nullptr);

    void initTable(BasicSql *db);

    bool refreshTable(const QString &table);
    void setStretch();
    void setColumnsHidden();
    void setColumnHidden(int column);
    int getList(QList<QStringList> &list);
    void insertRow(QStringList &list);
    int getCurrentId();

signals:
    void modifySig(int);
    void delSig(int);


public slots:
    void delSlot();
    void delSlot(int);
    void modifySlot();
    void clearTableSlot();
    void refreshSlot();
    void submitSlot();
    void querySlot(const QString &);    
    void doubleSlot(QModelIndex);

protected slots:
    void autoDelSlot();

public:
    BasicSql *mDb;
    SqlTableModel *model;

private:
    QTimer *timer;
    QTableView *tableView;
};

#endif // SQLTABLEVIEW_H
