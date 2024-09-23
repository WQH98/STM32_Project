#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 添加所有串口到comboBox里（串口号的名字叫comboBox_COMx）
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cbx_port_choose->addItem(info.portName()); //+':'+info.description()
    }
    g_serial = new QSerialPort(this);
    connect(g_serial, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btn_open_port_clicked()
{
    if(ui->btn_open_port->text()==tr("打开串口"))
    {
        ui->btn_open_port->setText(tr("关闭串口"));
        // 设置串口号（以当前显示的COM号为要打开的串口）
        g_serial->setPortName(ui->cbx_port_choose->currentText());
        // 以读写方式打开串口
        if(g_serial->open(QIODevice::ReadWrite))
        {
           qDebug() << "Open OK" << endl;
        }
        else
        {
           qDebug() << "Open Failed" << endl;
        }
        g_serial->setBaudRate(115200);
        //设置数据位
        g_serial->setDataBits(QSerialPort::Data8);
        //设置校验位
        g_serial->setParity(QSerialPort::NoParity);
        //设置流控制
        g_serial->setFlowControl(QSerialPort::NoFlowControl);
        //设置停止位
        g_serial->setStopBits(QSerialPort::OneStop);
    }
    else
    {
        ui->btn_open_port->setText(tr("打开串口"));
        g_serial->close();
    }
}

void Widget::on_btn_exit_clicked()
{
    Widget::close();
}

void Widget::on_ready_read() {
    QByteArray rec = g_serial->readAll();
    // qDebug() << rec << endl;
    ui->ted_show->append(rec.toHex());
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
