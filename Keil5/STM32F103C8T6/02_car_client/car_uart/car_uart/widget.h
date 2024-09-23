#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_open_port_clicked();

    void on_btn_exit_clicked();
    void on_ready_read();

private:
    Ui::Widget *ui;
    QSerialPort *g_serial;
};
#endif // WIDGET_H
