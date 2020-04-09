#ifndef BACKCOLOURCOM_H
#define BACKCOLOURCOM_H
#include <QWidget>
#include <QHeaderView>
#include <QColor>

namespace cm {

const QColor gray(174,225,254);//空设备,原为灰色，现为天蓝色
const QColor green(1,255,102);//正常
const QColor orange(255,0,0);//报警，原为橘红，现为红色
const QColor blue(200,200,200);//离线，原为男色，现为灰色
const QColor yellow(255, 255, 0);//预警，黄色

template <typename T>
bool bInBound(const T & min, const T & value, const T & max)
{
    return value >= min && value <= max;
}

}


QColor cm_temp2Color(int value);
QColor cm_color(int index);

/* 设置窗口背景色函数 */
void com_setBackColour(const QString &,QWidget *target);

void button_style_sheet(QWidget *target);
void btnBlue_style_sheet(QWidget *target);
void groupBox_background_icon(QWidget *target);
void set_background_icon(QWidget *widget, const QString &icon);
void set_background_color(QWidget *widget, const QColor &color);
void set_background_icon(QWidget *widget, const QString &icon, const QSize &size);

#endif // BACKCOLOURCOM_H
