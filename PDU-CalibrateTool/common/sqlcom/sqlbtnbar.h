#ifndef SQLBTNBAR_H
#define SQLBTNBAR_H

#include "sqldatebar.h"

namespace Ui {
class SqlBtnBar;
}

class SqlBtnBar : public QWidget
{
    Q_OBJECT

public:
    explicit SqlBtnBar(QWidget *parent = 0);
    ~SqlBtnBar();

    void gridLayout(QWidget *parent);
    void setEdit();
    void setNoEdit();
    void clearHidden();
    void queryHidden();
    void importHidden();

signals:
    void delSig();
    void delSig(int);

    void modifySig();
    void refreshSig();
    void clearSig();
    void exportSig();
    void querySig(const QString &);

protected:
    bool checkJur();
    virtual void addBtn(){}
    virtual void modifyBtn(int){}
    virtual bool delBtn(int){return true;}
    virtual bool importBtn(){return false;}
    virtual QString queryBtn(){return "";}

public slots:
    void delSlot(int);
    void modifySlot(int);


private slots:
    void on_addBtn_clicked();
    void on_modifyBtn_clicked();
    void on_queryBtn_clicked();
    void on_refreshBtn_clicked();
    void on_delBtn_clicked();
    void on_clearBtn_clicked();
    void on_exportBtn_clicked();
    void on_importBtn_clicked();

private:
    Ui::SqlBtnBar *ui;
};

#endif // SQLBTNBAR_H
