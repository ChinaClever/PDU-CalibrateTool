/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "backcolourcom.h"


/* 窗口背景色 */
#define     WIDGET_BACK_COLOR   232,249,255
#define     BUTTON_BACK_COLOR  232,238,250

#define     DILAG_BACK_COLOR      85,152,215
#define     DILAG_BUTTON_COLOR  255,255,255

/* 获得焦点BTN背景颜色 */
#define    BTN_FOCUS_BACK_COLOR    "QPushButton:focus{background-color: rgb(96,238,250);}"


QColor cm_color(int index)
{
    static QColor colorArray[] =
    {QColor(52,233,0),QColor(220,223,0),QColor(255,162,0),QColor(0,147,138),QColor(0,240,226),
     QColor(0,158,240),QColor(0,96,145),QColor(203,161,255),QColor(119,80,168),QColor(248,127,136),
     QColor(169,65,72),QColor(138,196,139),QColor(81,120,82)};

    index = index%(sizeof(colorArray)/sizeof(colorArray[0]));
    return colorArray[index];
}

QColor cm_temp2Color(int value)
{
    if(value>35)return QColor(255,90,0,255);
    if(value>30)return QColor(255,255,0,255);
    if(value>25)return QColor(100,200,105,255);
    if(value>22)return QColor(100,128,255,255);
    if(value>0)return QColor(0,128,255,255);
    return QColor(200,200,200,255);
}

/**
 * @brief 设置背景颜色
 * @param str 窗口标题名
 * @param target 目标窗口
 */
void com_setBackColour(const QString &str,QWidget *target)
{
    target->setWindowTitle(str);
    //target->setWindowFlags(Qt::FramelessWindowHint);
    target->setAutoFillBackground(true) ;
    QPalette paletteTool = target->palette() ;
    QBrush brushToolPic(QColor(WIDGET_BACK_COLOR)) ;
    QBrush brushToolwhite(QColor(BUTTON_BACK_COLOR)) ;
    paletteTool.setBrush(QPalette::Window,brushToolPic) ;
    paletteTool.setBrush(QPalette::Base,brushToolPic) ;
    paletteTool.setBrush(QPalette::Button,brushToolwhite) ;
    target->setPalette(paletteTool) ;

//    target->setStyleSheet(BTN_FOCUS_BACK_COLOR);
    target->setWindowIcon(QIcon(":/image/logo.jpg"));
}


/**
 * @brief 设备背景图标
 * @param widget 窗口
 * @param icon 图片路径
 */
void set_background_icon(QWidget *widget, const QString &icon)
{
    QPalette palette;
    QPixmap map = QPixmap(icon);
    palette.setBrush(QPalette::Background, QBrush(map));
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

/**
 * @brief 设备背景颜色
 * @param widget
 * @param color
 */
void set_background_color(QWidget *widget, const QColor &color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}


/**
 * @brief 设置背景图片
 * @param widget
 * @param icon
 * @param size
 */
void set_background_icon(QWidget *widget, const QString &icon,const QSize &size)
{
    QPalette palette;
    QPixmap map = QPixmap(icon).scaled(size);
    palette.setBrush(QPalette::Background, QBrush(map));
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

/**
 * @brief button 样式
 */
void button_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(102, 199, 249);}"
                          "QPushButton{font-family:'Microsoft YaHei';font-size:14px; color:rgb(0, 0, 0);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);}"
                          "QComboBox{background-color:rgb(255,255,255);}" );

    target->setFont(QFont("微软雅黑",12));
}

/**
 * @brief button 样式 背景色为蓝色，字体为白色
 */
void btnBlue_style_sheet(QWidget *target)
{
    target->setStyleSheet("QPushButton{background-color:rgb(12,62,180); color:rgb(255, 255, 255);}"
                          "QPushButton:hover{background-color:rgb(91, 237, 238);color:rgb(0, 0, 0);}"
                          "QPushButton:pressed{background-color:rgb(143, 185, 181);color:rgb(0, 0, 0);}" );
    target->setFont(QFont("微软雅黑",12));
}

/**
 * @brief 设置groupBox_background_icon
 * @param target
 */
void groupBox_background_icon(QWidget *target)
{
    set_background_icon(target,":/image/box_back.jpg");
//    btnBlue_style_sheet(target);
//    target->setStyleSheet(BTN_FOCUS_BACK_COLOR);
}
