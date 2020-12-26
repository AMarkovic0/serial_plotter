#include "plotter.h"
#include "ui_plotter.h"

plotter::plotter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::plotter),
    serial(new QSerialPort( ))
{
    ui->setupUi(this);

    // Set scroll interaction:
    ui->Plot->setInteraction( QCP::iRangeDrag , true );
    ui->Plot->setInteraction( QCP::iRangeZoom , true );

    // Setting graph:
    ui->Plot->addGraph( );
    ui->Plot->graph( 0 )->setScatterStyle( QCPScatterStyle::ssNone );
    ui->Plot->graph( 0 )->setLineStyle( QCPGraph::lsLine );
    ui->Plot->graph( 0 )->setPen( QPen( Qt::black ) );

    connect( serial , &QSerialPort::readyRead , this , &plotter::readData );
}

plotter::~plotter()
{
    serial->close();
    delete ui;
}

void plotter::on_bt_connect_clicked()
{
    serial->setPortName( ui->le_port->text( ) );
    serial->setBaudRate( ui->le_baud->text( ).toInt( ) );
    switch( ui->cb_dataBits->currentText( ).toInt( ) )
    {
        case 8:
            serial->setDataBits( QSerialPort::Data8 );
            break;
        case 7:
            serial->setDataBits( QSerialPort::Data7 );
            break;
        case 6:
            serial->setDataBits( QSerialPort::Data6 );
            break;
        case 5:
            serial->setDataBits( QSerialPort::Data5 );
            break;
        default:
            serial->setDataBits( QSerialPort::Data8 );
    }
    switch( ui->cb_parity->currentIndex( ) )
    {
        case 0:
            serial->setParity( QSerialPort::NoParity );
            break;
        case 1:
            serial->setParity( QSerialPort::EvenParity );
            break;
        case 2:
            serial->setParity( QSerialPort::OddParity );
            break;
        default:
            serial->setParity( QSerialPort::NoParity );
    }
    switch ( ui->cb_stopBits->currentIndex( ) )
    {
        case 0:
            serial->setStopBits( QSerialPort::OneStop );
            break;
        case 1:
            serial->setStopBits( QSerialPort::TwoStop );
            break;
        default:
            serial->setStopBits( QSerialPort::OneStop );
            break;
    }

    if( serial->open( QIODevice::ReadWrite ) )
    {
        printf( "prog: Connected\n" );
        ui->rcvTxt->setPlainText( "prog: Connected\n" );
    }
    else
    {
        printf("prog: Open error.\n");
        ui->rcvTxt->setPlainText( "prog: Connected\n" );
    }
}

void plotter::on_pushButton_clicked()
{
    serial->write( ui->le_cmd->text( ).toLocal8Bit( ) );
    ui->sendTxt->insertPlainText( ui->le_cmd->text( ) );
}

void plotter::on_bt_clear_clicked()
{
    clearData( );
    ui->rcvTxt->clear();
    ui->sendTxt->clear();

}

void plotter::addPoint( double x , double y )
{
    qv_x.append( x );
    qv_y.append( y );
}

void plotter::clearData( )
{
    qv_x.clear( );
    qv_y.clear( );
}

void plotter::plot( void )
{
    ui->Plot->graph( 0 )->setData( qv_x , qv_y );
    ui->Plot->replot( );
    ui->Plot->update( );
}

void plotter::readData( void )
{
    static QByteArray r_val;

    r_val.append( serial->readAll( ) );

    if( sizeof( float ) == r_val.length( ) )
    {
        if( 0 == ui->cb_rcvAs->currentIndex( ) )
        {
            addPoint( qv_y.length( ) , QString( r_val ).toFloat( ) );
            ui->rcvTxt->insertPlainText( r_val );
            ui->rcvTxt->insertPlainText( QString( " ; " ) );
        }
        else
        {
            for( uint8_t j = 0 ; j < sizeof( float ) ; j++ )
            {
                read_val.val_bytes[ j ] = r_val.data()[ j ];
            }
            addPoint( qv_y.length( ) , read_val.val );
            ui->rcvTxt->insertPlainText( QString::number( read_val.val ) );
            ui->rcvTxt->insertPlainText( QString( " ; " ) );
        }

        ui->Plot->xAxis->setRange( *std::max_element( qv_x.begin( ) , qv_x.end( ) ) - ui->le_wsize->text().toInt() ,
                                       *std::max_element( qv_x.begin( ) , qv_x.end( ) ) );
        ui->Plot->yAxis->setRange( *std::min_element( qv_y.begin( ) , qv_y.end( ) ) ,
                                       *std::max_element( qv_y.begin( ) , qv_y.end( ) ) );
        plot( );
        r_val.clear();
    }
}
