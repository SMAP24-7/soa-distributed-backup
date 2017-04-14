#include "cliente.h"
#include "ui_cliente.h"

#include <QTcpSocket>
#include <QTextStream>

Cliente::Cliente(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cliente)
{

    ui->setupUi(this);

    ui->conectar->setDisabled(true);
    ui->pushFolder->setDisabled(true);
    ui->maxclient->setHidden(true);
    ui->Origen->setHidden(true);
    ui->Destino->setHidden(true);

    mSocket = new QTcpSocket(this);

}

Cliente::~Cliente()
{
        delete ui;
}

void Cliente::read_block()
{
    qDebug() << "mensaje de entrada";

    QDataStream read(mSocket->read(mSocket->bytesAvailable()));

    quint64 bytesize;
    quint64 size = 0;
    QString bit;
    int num;
    QString name;
    QByteArray contain;

    read >> bytesize;

    if (bytesize == 1){
        read >> size;
        size_files_ = size;
    }
    else{
        read >> bit;
        read >> num;

        sender_block_ += bytesize;
        QDir dir(data_.second + bit);
        if (!dir.exists()){
            QDir dir_folder = dir;
            QString new_folder = dir.absoluteFilePath("");
            dir_folder.mkdir(new_folder);
        }
        for (int q = 0; q < num ; q++){
            read >> name;
            read >> contain;
            QFile file(dir.absoluteFilePath(name));
            if (file.open(QIODevice::WriteOnly)){
                file.write(contain);
            }
        }
        ui->progressBar->setValue(sender_block_ / size_files_ *100);
    }
}

void Cliente::send_initial_block()
{
    qDebug() << "enviando el bloque inicial";
    if (mSocket){
        cpy_mSocket = mSocket;
        QByteArray block_initial = NULL;
        int num_conexiones = ui->maxclient->value();

        QDataStream out(&block_initial, QIODevice::WriteOnly);
        QDir dir(data_.second);
        size_block(size_files_, dir);

        out << (quint64)0;
        out << num_conexiones;
        out << size_files_;
        out.device()->seek(0);

        out << quint64((2*sizeof(quint64) - sizeof(quint64)));
        mSocket->write(block_initial);
        mSocket->flush();
        connect(mSocket,SIGNAL(readyRead()),this,SLOT(send_all_block()));
    }
}

void Cliente::on_conectar_clicked()
{
    ui->radio_origen->setEnabled(false);
    ui->radio_destino->setEnabled(false);
    get_ruta();

    blockSize_ = 0;
    mSocket->abort();
    mSocket->connectToHost(ui->name_server->text(), ui->puerto->value());
    mSocket->setReadBufferSize(Buffer_);
}

void Cliente::on_cerrar_clicked()
{
    close();
}

void Cliente::get_ruta()
{
    if (ui->radio_destino->isChecked()){
       connect(mSocket,SIGNAL(readyRead()),this,SLOT(read_block()));
       qDebug() << "escuchando";
    }
    else if (ui->radio_origen->isChecked()){
       connect(mSocket,SIGNAL(connected()),this,SLOT(send_initial_block()));
       qDebug() << "voy a enviar";
       }

}

void Cliente::on_radio_destino_clicked()
{
    ui->pushFolder->setEnabled(true);
    ui->Origen->setText("Cliente");
    ui->Destino->setText("Servidor");
    ui->Origen->setHidden(false);
    ui->Destino->setHidden(false);
    ui->maxclient->setEnabled(false);
}

void Cliente::on_radio_origen_clicked()
{
    ui->pushFolder->setEnabled(true);
    ui->Origen->setText("Servidor");
    ui->Destino->setText("Cliente");
    ui->Origen->setHidden(false);
    ui->Destino->setHidden(false);
    ui->maxclient->setHidden(false);
}

void Cliente::size_block(quint64 &byte, QDir dir)
{
    QStringList list_dir = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList list_files = dir.entryList(QDir::Files);

    int num_dir = list_dir.size();
    int num_files = list_files.size();

    if (num_files != 0){
        for (int q = 0; q < num_files ; q++){
            QFile file_(dir.filePath(list_files[q]));
            byte += file_.size();
        }
    }
    if (num_dir != 0){
        for (int q = 0; q < num_dir ; q++){
            size_block(byte,dir.filePath(list_dir[q]));
        }
    }
}

void Cliente::do_block(QString relative_dir, QDir dir)
{
    QStringList list_dir = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList list_files = dir.entryList(QDir::Files);

    int num_dir = list_dir.size();
    int num_files = list_files.size();
    quint64 byte = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    relative_dir += "/" + dir.dirName();

    out << (quint64)0;      // Inicio del bloque 1/5    quint
    out << relative_dir;    // Path relativo 2/5    qstring
    qDebug () << "dirección relativa --> " << relative_dir;
    out << num_files;       // Número de archivos 3/5   int
    qDebug() << "N archivos --> " << num_files;


    for (int q = 0; q < num_files ; q++){
        QFile file_(dir.filePath(list_files[q]));
        QFileInfo f(file_);

        if (file_.open(QIODevice::ReadOnly)){
            QByteArray cpy;
            cpy = file_.readAll();
            out << f.fileName();
            out << cpy; //Archivo 5/5   qbytearray
            byte += file_.size();
        }
    }

    qDebug() << "ENVIADO";
    out.device()->seek(0);
    out << byte + sizeof(quint64);
    sender_block_ += byte;
    if (mSocket->readBufferSize() > mSocket->bytesToWrite()){
        mSocket->write(block);
        ui->progressBar->setValue(sender_block_ / size_files_ *100);
        mSocket->waitForReadyRead(300);
        mSocket->flush();

        if (num_dir != 0){
            for (int q = 0; q < num_dir ; q++){
                do_block(relative_dir, dir.filePath(list_dir[q]));
            }
        }
    }
    else {
        mSocket->waitForReadyRead(3000);
        do_block(relative_dir, dir);
    }
}

void Cliente::send_all_block()
{
        qDebug() << "Hay clientes";

        QDataStream read(mSocket->read(mSocket->bytesAvailable()));
        quint64 data;
        int client;
        read >> data;
        read >> client;
        qDebug() << sizeof(read) << "hay " << client << " clientes";

        QDir drs(data_.second);
        do_block("",drs);
}

void Cliente::on_pushFolder_clicked()
{
     data_.second = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
     ui->Direrctorio->setText(data_.second);
     ui->conectar->setEnabled(true);
}
