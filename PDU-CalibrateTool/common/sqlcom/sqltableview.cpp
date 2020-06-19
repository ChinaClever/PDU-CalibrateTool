/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "sqltableview.h"

SqlTableView::SqlTableView(QWidget *parent) : QWidget(parent)
{
    tableView = new QTableView(this);
    tableView->setSortingEnabled(true);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁用编辑功能
    tableView->resizeColumnsToContents();
    tableView->horizontalHeader()->setStretchLastSection(true);
    // tableView->setAlternatingRowColors(true);
    // tableView->setStyleSheet("QTableView{ background-color: rgb(202, 232, 234); alternate-background-color: rgb(245, 245, 245); }");
    // connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleSlot(QModelIndex)));

    mDb = nullptr;
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(tableView);

    timer = new QTimer(this);
    timer->start(4*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(autoDelSlot()));
    QTimer::singleShot(60*1000,this,SLOT(autoDelSlot()));
}


/**
 * @brief 初始化表格
 */
void SqlTableView::initTable(BasicSql *db)
{
    model = new SqlTableModel(tableView, db->mDb);
    tableView->setModel(model->model);
    this->mDb = db;
    refreshSlot();
}


/**
 * @brief 列平均分配
 */
void SqlTableView::setStretch()
{
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


int SqlTableView::getCurrentId()
{
    int id = -1;
    int curRow = tableView->currentIndex().row();
    if(curRow >= 0) {
        QModelIndex index = model->model->index(curRow, 0);
        id = model->model->data(index, Qt::DisplayRole).toInt();
    }
    return id;
}

void SqlTableView::modifySlot()
{
    int id = getCurrentId();
    if(id >= 0) emit modifySig(id);
}


void SqlTableView::submitSlot()
{
    bool ret = model->submitChanges();
    if(!ret) {
        QMessageBox::warning(this, mDb->tableName(),
                             tr("数据库错误: %1").arg(model->model->lastError().text()));
    }
}

void SqlTableView::querySlot(const QString &str)
{
    model->queryFilter(str);
}

/**
 * @brief 插入一行
 * @param list
 */
void SqlTableView::insertRow(QStringList &list)
{
    int rowNum = model->model->rowCount(); //获得表的行数
    model->model->insertRow(rowNum); //添加一行

    int id = mDb->maxId() + 1;
    QModelIndex index = model->model->index(rowNum,0);
    model->model->setData(index,id);
    for(int i=0; i< list.size(); ++i) {
        index = model->model->index(rowNum,i+1);
        model->model->setData(index,list.at(i));
    }
    //    submitSlot();
}

/**
 * @brief 刷新表格
 * @param table 表格名
 * @return
 */
bool SqlTableView::refreshTable(const QString &table)
{
    bool ret = model->refreshTable(table);
    if(ret) {
        tableView->sortByColumn(0, Qt::DescendingOrder); // 降序排列
        setColumnsHidden();
    }
    return  ret;
}

void SqlTableView::setColumnsHidden()
{
    for(int i=0; i<mDb->hiddens.size(); ++i) {
        int column = mDb->hiddens.at(i);
        setColumnHidden(column);
    }
}

void SqlTableView::setColumnHidden(int column)
{
    tableView->setColumnHidden(column, true);
}

void SqlTableView::refreshSlot()
{
    QString table = mDb->tableName();
    this->refreshTable(table);
    model->setHeaders(mDb->headList);
}

void SqlTableView::autoDelSlot()
{
//    int size = ConfigBase::bulid()->item->logCount;

    int size = 10*10000;  /////==========
    int count = mDb->counts();
    if(count > size) {
        refreshSlot();
        mDb->countsRemove(size);
    }
}

/**
 * @brief 清空数据库
 */
void SqlTableView::clearTableSlot()
{
    mDb->clear();
    QCoreApplication::processEvents(QEventLoop::AllEvents,450);
    refreshSlot();
}

/**
 * @brief 删除一条纪录
 */
void SqlTableView::delSlot()
{
    int id = getCurrentId();
    if(id >= 0) emit delSig(id);
}

void SqlTableView::delSlot(int)
{
    int curRow = tableView->currentIndex().row();
    model->removeRow(curRow);
}

void SqlTableView::doubleSlot(QModelIndex)
{
    QString str = tr("是否删除这条纪录?");
    QuMsgBox box(this, str);
    bool ret = box.Exec();
    if(ret) {
        delSlot();
    }
}

int SqlTableView::getList(QList<QStringList> &list)
{
    QStringList head;
    head << tr("编号");
    for(int i=0; i<mDb->headList.size(); ++i) {
        if(mDb->hiddens.contains(i)) continue;
        head << mDb->headList.at(i);
    }
    list.append(head);

    int row = model->model->rowCount();
    for(int j=0; j<row; ++j) {
        QStringList strList;
        strList << QString::number(j+1);
        int column = model->model->columnCount();
        for(int i=0; i<column; ++i)
        {
            if(mDb->hiddens.contains(i)) continue;
            QModelIndex index = model->model->index(j, i);
            strList << model->model->data(index, Qt::DisplayRole).toString();
        }
        list.append(strList);
    }
    return list.size();
}

