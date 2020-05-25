#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialstatuswid.h"
#include "modeselectwid.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_readmeBtn_clicked();

private:
    Ui::MainWindow *ui;
    DataTableWid *mTableWid;
    ModeSelectWid *mModeWid;
    SerialStatusWid *mSerialWid;
    SerialStatusWid *mSourceWid;
};
#endif // MAINWINDOW_H
