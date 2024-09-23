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
    QPushButton *bt_start_listen;
    QPushButton *bt_stop_listen;
    QPushButton *bt_clear_message;
    QLabel *label;
    QComboBox *cbx_listen_address;
    QLabel *label_2;
    QLineEdit *let_listen_port;
    QTextEdit *tex_show;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *led_green_led;
    QLineEdit *led_white_led;
    QLineEdit *led_pitch;
    QLineEdit *led_roll;
    QLineEdit *led_yaw;
    QLineEdit *led_temp;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(557, 653);
        bt_start_listen = new QPushButton(Widget);
        bt_start_listen->setObjectName(QString::fromUtf8("bt_start_listen"));
        bt_start_listen->setGeometry(QRect(30, 10, 75, 23));
        bt_stop_listen = new QPushButton(Widget);
        bt_stop_listen->setObjectName(QString::fromUtf8("bt_stop_listen"));
        bt_stop_listen->setGeometry(QRect(120, 10, 75, 23));
        bt_clear_message = new QPushButton(Widget);
        bt_clear_message->setObjectName(QString::fromUtf8("bt_clear_message"));
        bt_clear_message->setGeometry(QRect(210, 10, 75, 23));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 60, 54, 12));
        cbx_listen_address = new QComboBox(Widget);
        cbx_listen_address->setObjectName(QString::fromUtf8("cbx_listen_address"));
        cbx_listen_address->setGeometry(QRect(100, 50, 181, 22));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(290, 50, 54, 12));
        let_listen_port = new QLineEdit(Widget);
        let_listen_port->setObjectName(QString::fromUtf8("let_listen_port"));
        let_listen_port->setGeometry(QRect(350, 50, 113, 20));
        tex_show = new QTextEdit(Widget);
        tex_show->setObjectName(QString::fromUtf8("tex_show"));
        tex_show->setGeometry(QRect(40, 120, 401, 251));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(70, 400, 54, 12));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(70, 420, 54, 12));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(70, 450, 54, 12));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(70, 480, 54, 12));
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(70, 510, 54, 12));
        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(70, 540, 54, 12));
        led_green_led = new QLineEdit(Widget);
        led_green_led->setObjectName(QString::fromUtf8("led_green_led"));
        led_green_led->setGeometry(QRect(130, 400, 113, 20));
        led_white_led = new QLineEdit(Widget);
        led_white_led->setObjectName(QString::fromUtf8("led_white_led"));
        led_white_led->setGeometry(QRect(130, 420, 113, 20));
        led_pitch = new QLineEdit(Widget);
        led_pitch->setObjectName(QString::fromUtf8("led_pitch"));
        led_pitch->setGeometry(QRect(130, 440, 113, 20));
        led_roll = new QLineEdit(Widget);
        led_roll->setObjectName(QString::fromUtf8("led_roll"));
        led_roll->setGeometry(QRect(120, 470, 113, 20));
        led_yaw = new QLineEdit(Widget);
        led_yaw->setObjectName(QString::fromUtf8("led_yaw"));
        led_yaw->setGeometry(QRect(130, 500, 113, 20));
        led_temp = new QLineEdit(Widget);
        led_temp->setObjectName(QString::fromUtf8("led_temp"));
        led_temp->setGeometry(QRect(130, 530, 113, 20));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        bt_start_listen->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\347\233\221\345\220\254", nullptr));
        bt_stop_listen->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242\347\233\221\345\220\254", nullptr));
        bt_clear_message->setText(QCoreApplication::translate("Widget", "\346\270\205\347\251\272", nullptr));
        label->setText(QCoreApplication::translate("Widget", "\347\233\221\345\220\254\345\234\260\345\235\200", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\347\233\221\345\220\254\347\253\257\345\217\243", nullptr));
        let_listen_port->setText(QCoreApplication::translate("Widget", "5001", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\347\273\277\347\201\257", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\347\231\275\347\201\257", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "\344\277\257\344\273\260\350\247\222", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\347\277\273\346\273\232\350\247\222", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "\345\201\217\350\210\252\350\247\222", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "\346\270\251\345\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
