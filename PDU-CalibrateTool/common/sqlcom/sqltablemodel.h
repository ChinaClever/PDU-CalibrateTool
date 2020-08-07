#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QWidget>
#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include "basicsql.h"

class SqlTableModel : public QWidget
{
    Q_OBJECT
public:
    explicit SqlTableModel(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase());
    
    void setHeaders(QStringList &);

    bool removeRow(int);
    bool refreshTable(const QString &);

    void ascendingOrder(int);
    void descendingOrder(int);

    bool submitChanges(void);
    void revertChanges(void);
    bool removeRows();

signals:
    
public slots:
    void queryFilter(const QString &);

public:
   QSqlRelationalTableModel *model;
};

#endif // SQLTABLEMODEL_H
