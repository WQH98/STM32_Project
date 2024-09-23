#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QDebug>
#include "string.h"

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
    void on_bt_start_listen_clicked();
    void on_bt_stop_listen_clicked();
    void on_bt_clear_message_clicked();
    void on_newconnection();
    void on_readyread();
private:
    Ui::Widget *ui;
    QTcpServer *g_server;   //监听的套接字
    QTcpSocket *g_socket;   //通信的套接字
};
#endif // WIDGET_H
