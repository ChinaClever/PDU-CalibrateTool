/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "sqltableview.h"
#include "ad_config.h"

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
    tableView->horizontalHeader()->setStyleSheet("font: 12pt \"微软雅黑\";");
    tableView->setStyleSheet("font: 12pt \"微软雅黑\";");
    // initScrollArea(); // 开启滑动功能

    mDb = nullptr;
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(tableView);

    timer = new QTimer(this);
    timer->start(6*60*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(autoDelSlot()));
    QTimer::singleShot(10*1000,this,SLOT(autoDelSlot()));

}

void SqlTableView::initScrollArea()
{
    m_scrollBarV = tableView->verticalScrollBar();
    QObjectList objectList = tableView->children();
    for(int i = 0; i < objectList.count(); i++) {
        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport") {
            objectList.at(i)->installEventFilter(this);
        }
    }
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

bool SqlTableView::eventFilter(QObject *obj, QEvent *event)
{
    static int press_y   = 0;
    static int move_y    = -1;
    static int release_y = 0;
    static QDateTime pressDateTime;
    static QPropertyAnimation *animation = new QPropertyAnimation();
    if("qt_scrollarea_viewport" != obj->objectName()) return false;

    int scrollV_max = m_scrollBarV->maximum();
    int scrollV_min = m_scrollBarV->minimum();

    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
    if(event->type() == QEvent::MouseButtonPress) {  //记录按下的时间、坐标
        pressDateTime = QDateTime::currentDateTime();
        move_y  = QCursor::pos().y();
        press_y = move_y;
        animation->stop();
    } else if(event->type() == QEvent::MouseButtonRelease) {
        //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离
        if(animation->targetObject() != m_scrollBarV) {
            animation->setTargetObject(m_scrollBarV);
            animation->setPropertyName("value");
        }

        move_y = -1;
        release_y = QCursor::pos().y();
        QObject *parent_obj = obj->parent();
        if(parent_obj != 0 || parent_obj->inherits("QAbstractItemView")) {
            QTimer::singleShot(150, (QAbstractItemView *)parent_obj, SLOT(clearSelection()));
        }

        int endValue = 0;
        int pageStep;
        if(release_y - press_y != 0 && qAbs(release_y - press_y) > 45) {
            int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
            int limit = 440;
            pageStep = 240;//scrollBarV->pageStep();
            if(mseconds > limit) { //滑动的时间大于某个值的时候，不再滚动(通过增加分母)
                mseconds = mseconds + (mseconds - limit) * 20;
            }

            if(release_y - press_y > 0) {//.0避免避免强制转换为整形
                endValue = m_scrollBarV->value() - pageStep * (200.0 / mseconds);
                if(scrollV_min > endValue) endValue = scrollV_min;
            } else if(release_y - press_y < 0) {
                endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
                if(endValue > scrollV_max) endValue = scrollV_max;
            }
            if(mseconds > limit) mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
            animation->setDuration(mseconds+550);
            animation->setEndValue(endValue);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            animation->start();
            return true;
        }
    } else if(event->type() == QEvent::MouseMove && move_y >= 0) {   //窗口跟着鼠标移动
        int move_distance = QCursor::pos().y() - move_y;
        int endValue = m_scrollBarV->value() - move_distance;
        if(scrollV_min > endValue) endValue = scrollV_min;
        if(endValue > scrollV_max)  endValue = scrollV_max;
        m_scrollBarV->setValue(endValue);
        move_y = QCursor::pos().y();
    }

    return false;
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
    int size = Ad_Config::bulid()->item->logCount;
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
 * @brief 删除一条记录
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
    QString str = tr("是否删除这条记录?");
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

