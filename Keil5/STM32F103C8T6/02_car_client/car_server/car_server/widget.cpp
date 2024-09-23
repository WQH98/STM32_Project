#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tex_show->append("server start");
    // 本地主机名
    QString host_name = QHostInfo::localHostName();

    // 本机IP地址
    QHostInfo host_info = QHostInfo::fromName(host_name);

    // 设置IP地址列表
    QList<QHostAddress> addr_list = host_info.addresses();
    for(int i = 0; i < addr_list.count(); i++) {
        QHostAddress host = addr_list.at(i);
        if(QAbstractSocket::IPv4Protocol == host.protocol()) {
            QString ip = host.toString();
            ui->cbx_listen_address->addItem(ip);
        }
    }
    ui->bt_start_listen->setEnabled(true);
    ui->bt_stop_listen->setEnabled(false);
    ui->cbx_listen_address->setEnabled(true);
    // 实例化server和socket
    g_server = new QTcpServer(this);
    g_socket = new QTcpSocket(this);
    // 当有新连接信号的时候 会进这个槽函数
    connect(g_server, SIGNAL(newConnection()), this, SLOT(on_newconnection()));
}

Widget::~Widget()
{
    delete ui;
}


// 开始监听按钮的槽函数
void Widget::on_bt_start_listen_clicked()
{
    // 当前选择的IP
    QString ip = ui->cbx_listen_address->currentText();
    // 选择当前的端口
    int port = ui->let_listen_port->text().toInt();
    // 开始监听
    g_server->listen(QHostAddress(ip), port);
    ui->bt_start_listen->setEnabled(false);
    ui->bt_stop_listen->setEnabled(true);
    ui->cbx_listen_address->setEnabled(false);
}

// 停止监听按钮的槽函数
void Widget::on_bt_stop_listen_clicked()
{
    // if(g_server->isListening()) {
        g_server->close();
        g_socket->close();
        ui->bt_start_listen->setEnabled(true);
        ui->bt_stop_listen->setEnabled(false);
        ui->cbx_listen_address->setEnabled(true);
    // }
}

// 清空按钮的槽函数
void Widget::on_bt_clear_message_clicked()
{
    ui->tex_show->clear();
}

// 新连接信号的槽函数
void Widget::on_newconnection() {
    ui->tex_show->append("client connect");
    g_socket = g_server->nextPendingConnection();
    connect(g_socket, SIGNAL(readyRead()), this, SLOT(on_readyread()));
    ui->tex_show->append("client ip = " + g_socket->peerAddress().toString());
    ui->tex_show->append("client port = " + QString::number(g_socket->peerPort()));
}

// 有新数据可以读出的槽函数
void Widget::on_readyread() {
    QByteArray rec = g_socket->readAll();
    int rec_len = rec.length();
    int sum = 0;
    float fdata = 0;
    char data[4];
    if((int)rec[0] == 0x55) {
        for(int i = 0; i < rec_len; i++) {
            sum += rec[i];
            if((int)rec[rec_len - 1] == sum) {
                // 数据解析开始
                if((int)rec[1] == 0x01) {
                    ui->led_green_led->setText("开");
                }
                else {
                    ui->led_green_led->setText("关");
                }
                if((int)rec[2] == 0x01) {
                    ui->led_white_led->setText("开");
                }
                else {
                    ui->led_white_led->setText("关");
                }
                data[0] = rec[3];
                data[1] = rec[4];
                data[2] = rec[5];
                data[3] = rec[6];
                memcpy(&fdata, data, sizeof(fdata));
                ui->led_pitch->setText(QString::number(fdata));
                data[0] = rec[7];
                data[1] = rec[8];
                data[2] = rec[9];
                data[3] = rec[10];
                memcpy(&fdata, data, sizeof(fdata));
                ui->led_roll->setText(QString::number(fdata));
                data[0] = rec[11];
                data[1] = rec[12];
                data[2] = rec[13];
                data[3] = rec[14];
                memcpy(&fdata, data, sizeof(fdata));
                ui->led_yaw->setText(QString::number(fdata));
                data[0] = rec[15];
                data[1] = rec[16];
                data[2] = rec[17];
                data[3] = rec[18];
                memcpy(&fdata, data, sizeof(fdata));
                ui->led_temp->setText(QString::number(fdata / 100));
            }
        }
    }
}
