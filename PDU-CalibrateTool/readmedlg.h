#ifndef READMEDLG_H
#define READMEDLG_H

#include <QDialog>

namespace Ui {
class ReadMeDLg;
}

class ReadMeDLg : public QDialog
{
    Q_OBJECT

public:
    explicit ReadMeDLg(QWidget *parent = nullptr);
    ~ReadMeDLg();

protected:
    void initText();

private slots:
    void on_okBtn_clicked();

private:
    Ui::ReadMeDLg *ui;
};

#endif // READMEDLG_H
