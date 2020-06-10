#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "navbarwid.h"
#include "home_mainwid.h"
#include "logmainwid.h"
#include "setup_mainwid.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initWid();

protected slots:
    void initFunSlot();
    void navBarSlot(int);

private slots:
    void on_readmeBtn_clicked();

private:
    Ui::MainWindow *ui;

    LogMainWid *mLog;
    NavBarWid *mNavBarWid;
    Home_MainWid *mHomeWid;
    Setup_MainWid *mSetupWid;
};
#endif // MAINWINDOW_H
