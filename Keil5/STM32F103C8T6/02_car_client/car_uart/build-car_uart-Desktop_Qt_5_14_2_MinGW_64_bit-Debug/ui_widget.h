/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *cbx_port_choose;
    QComboBox *cbx_boud;
    QComboBox *cbx_stop_bits;
    QComboBox *cbx_data_bits;
    QComboBox *cbx_parity_check;
    QPushButton *btn_open_port;
    QLineEdit *led_green_led;
    QLineEdit *led_white_led;
    QLineEdit *led_pitch;
    QLineEdit *led_roll;
    QLineEdit *led_yaw;
    QLineEdit *led_temp;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *btn_exit;
    QTextEdit *ted_show;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(652, 457);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 54, 16));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 60, 54, 16));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 100, 54, 16));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 140, 54, 16));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 180, 54, 16));
        cbx_port_choose = new QComboBox(Widget);
        cbx_port_choose->setObjectName(QString::fromUtf8("cbx_port_choose"));
        cbx_port_choose->setGeometry(QRect(110, 20, 131, 22));
        cbx_boud = new QComboBox(Widget);
        cbx_boud->setObjectName(QString::fromUtf8("cbx_boud"));
        cbx_boud->setGeometry(QRect(110, 50, 131, 22));
        cbx_stop_bits = new QComboBox(Widget);
        cbx_stop_bits->setObjectName(QString::fromUtf8("cbx_stop_bits"));
        cbx_stop_bits->setGeometry(QRect(110, 90, 131, 22));
        cbx_data_bits = new QComboBox(Widget);
        cbx_data_bits->setObjectName(QString::fromUtf8("cbx_data_bits"));
        cbx_data_bits->setGeometry(QRect(110, 130, 131, 22));
        cbx_parity_check = new QComboBox(Widget);
        cbx_parity_check->setObjectName(QString::fromUtf8("cbx_parity_check"));
        cbx_parity_check->setGeometry(QRect(110, 170, 131, 22));
        btn_open_port = new QPushButton(Widget);
        btn_open_port->setObjectName(QString::fromUtf8("btn_open_port"));
        btn_open_port->setGeometry(QRect(70, 230, 75, 23));
        led_green_led = new QLineEdit(Widget);
        led_green_led->setObjectName(QString::fromUtf8("led_green_led"));
        led_green_led->setGeometry(QRect(460, 30, 113, 20));
        led_white_led = new QLineEdit(Widget);
        led_white_led->setObjectName(QString::fromUtf8("led_white_led"));
        led_white_led->setGeometry(QRect(460, 60, 113, 20));
        led_pitch = new QLineEdit(Widget);
        led_pitch->setObjectName(QString::fromUtf8("led_pitch"));
        led_pitch->setGeometry(QRect(460, 90, 113, 20));
        led_roll = new QLineEdit(Widget);
        led_roll->setObjectName(QString::fromUtf8("led_roll"));
        led_roll->setGeometry(QRect(460, 120, 113, 20));
        led_yaw = new QLineEdit(Widget);
        led_yaw->setObjectName(QString::fromUtf8("led_yaw"));
        led_yaw->setGeometry(QRect(460, 150, 113, 20));
        led_temp = new QLineEdit(Widget);
        led_temp->setObjectName(QString::fromUtf8("led_temp"));
        led_temp->setGeometry(QRect(460, 190, 113, 20));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(380, 30, 54, 12));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(380, 62, 54, 20));
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(380, 100, 54, 12));
        label_9 = new QLabel(Widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(380, 130, 54, 12));
        label_10 = new QLabel(Widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(380, 160, 54, 12));
        label_11 = new QLabel(Widget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(380, 190, 54, 12));
        btn_exit = new QPushButton(Widget);
        btn_exit->setObjectName(QString::fromUtf8("btn_exit"));
        btn_exit->setGeometry(QRect(170, 230, 75, 23));
        ted_show = new QTextEdit(Widget);
        ted_show->setObjectName(QString::fromUtf8("ted_show"));
        ted_show->setGeometry(QRect(30, 280, 561, 171));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\345\212\251\346\211\213", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\351\200\211\346\213\251\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242\344\275\215\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\225\260\346\215\256\344\275\215\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "\345\245\207\345\201\266\346\240\241\351\252\214\357\274\232", nullptr));
        btn_open_port->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\347\273\277\347\201\257", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "\347\231\275\347\201\257", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "pitch", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "roll", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "yaw", nullptr));
        label_11->setText(QCoreApplication::translate("Widget", "tmp", nullptr));
        btn_exit->setText(QCoreApplication::translate("Widget", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
