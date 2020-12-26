#ifndef PLOTTER_H
#define PLOTTER_H

#include<stdint.h>

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#define SERIAL_BUFFER_SIZE

typedef union
{
    float val;
    uint8_t val_bytes[ 4 ];
}float_byte_u;

namespace Ui {
class plotter;
}

class plotter : public QMainWindow
{
    Q_OBJECT

public:
    explicit plotter(QWidget *parent = 0);
    ~plotter();
    void addPoint( double x , double y );
    void clearData( void );
    void plot( void );
    void readData( void );

private slots:
    void on_bt_connect_clicked();
    void on_pushButton_clicked();

    void on_bt_clear_clicked();

private:
    Ui::plotter *ui;

    QVector<double> qv_x;
    QVector<double> qv_y;
    float_byte_u read_val;

    QSerialPort* serial;
    uint8_t serial_buffer[ SERIAL_BUFFER_SIZE ];
};

#endif // PLOTTER_H
