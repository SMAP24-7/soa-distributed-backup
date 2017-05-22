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
    mSocket->disconnected();
    delete ui;
}

void Cliente::read_block()
{
    qDebug() << "mensaje de entrada";

    QByteArray block(mSocket->read(mSocket->bytesAvailable()));
    QDataStream read(&block,QIODevice::ReadOnly);

    int bytesize;

    quint64 size = 0;
    QString bit;
    int num;
    QString name;
    QByteArray contain;

    read >> bytesize;
    qDebug() << block.size() << "byte llegados de " << bytesize << " esperados";

    if (bytesize == 1 || bytesize == 0){
        qDebug() << "Archivo de cabecera recibido";
        read >> num;
        read >> size;
        qDebug() << "Numero de archivos --> "<< num;
        qDebug() << "Tamaño total -->" << size;
        size_files_ = size;
    }
    else if(bytesize < block.size()){
        qDebug() << bytesize << "llegados en un bloque de " << block.size();
        int bloque_exceso = bytesize;
        while (bloque_exceso < block.size()){
            read >> bit;
            read >> num;

        QDir dir(data_.second + bit);
        if (!dir.exists()){
            QDir dir_folder = dir;
            QString new_folder = dir.absoluteFilePath("");
            dir_folder.mkdir(new_folder);

        }
        for (int q = 0; q < num ; q++){
            read >> name;
            read >> contain;
            ui->listWidget->addItem(name);
            QFile file(dir.absoluteFilePath(name));
            if (file.open(QIODevice::WriteOnly)){
                file.write(contain);
                file.close();
            }
        }
        ui->progressBar->setValue(sender_block_ / size_files_ *100);
        int sum;
        read >> sum;
        sender_block_ += sum;
        bloque_exceso += sum;
    }
    }
    else if (bytesize > block.size()){
        QByteArray buffer_llegada;
        buffer_llegada = block;
        while (buffer_llegada.size() < bytesize){
            mSocket->waitForReadyRead(20);
            if (mSocket->bytesAvailable()){
                QByteArray block_faltante(mSocket->read((quint64)bytesize - (quint64)buffer_llegada.size()));
                buffer_llegada += block_faltante;
                sender_block_+=buffer_llegada.size();
                ui->progressBar->setValue(sender_block_ / size_files_ *100);
                qDebug() << buffer_llegada.size() << " de " << bytesize;
            }
        }
        qDebug() << buffer_llegada.size() << "byte obtenidos";
        QDataStream reader(&buffer_llegada,QIODevice::ReadOnly);
        reader >> bytesize;
        reader >> bit;
        reader >> num;

        QDir dir(data_.second + bit);
        if (!dir.exists()){
            QDir dir_folder = dir;
            QString new_folder = dir.absoluteFilePath("");
            dir_folder.mkdir(new_folder);
        }
        for (int q = 0; q < num ; q++){
            reader >> name;
            reader >> contain;
            ui->listWidget->addItem(name);
            QFile file(dir.absoluteFilePath(name));
            if (file.open(QIODevice::WriteOnly)){
                file.write(contain);
                file.close();
            }
        }
        mSocket->waitForReadyRead(20);
        if (mSocket->bytesAvailable()){
            read_block();
        }

    }
    if (bytesize == block.size() || bytesize < block.size()){
        qDebug() << block.size() << "Recibido bloque entero";
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
        ui->listWidget->addItem(name);
        QFile file(dir.absoluteFilePath(name));
        if (file.open(QIODevice::WriteOnly)){
            //QByteArray un_cmp = qUncompress(contain);
            file.write(contain);
            file.close();
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
        int bytesize = 1;

        QDataStream out(&block_initial, QIODevice::WriteOnly);
        QDir dir(data_.second);
        size_block(size_files_, dir);

        out << bytesize;
        out << num_conexiones;
        out << size_files_;
        qDebug() << "serán " <<
        //out.device()->seek(0);

        mSocket->write(block_initial);
        mSocket->flush();
        connect(mSocket,SIGNAL(readyRead()),this,SLOT(send_all_block()));
    }
}

void Cliente::on_conectar_clicked()
{
    blockSize_ = 0;
    mSocket->abort();
    mSocket->connectToHost(ui->name_server->text(), ui->puerto->value());
    mSocket->setReadBufferSize(Buffer_);
    if (mSocket->waitForConnected(10)){
        ui->radio_origen->setEnabled(false);
        ui->radio_destino->setEnabled(false);
        ui->conectar->setEnabled(false);
        get_ruta();
    }
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
       send_initial_block();
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
    ui->maxclient->setHidden(true);
    ui->label_4->setHidden(true);
}

void Cliente::on_radio_origen_clicked()
{
    ui->pushFolder->setEnabled(true);
    ui->Origen->setText("Servidor");
    ui->Destino->setText("Cliente");
    ui->Origen->setHidden(false);
    ui->Destino->setHidden(false);
    ui->maxclient->setEnabled(true);
    ui->maxclient->setHidden(false);
    ui->label_4->setHidden(false);
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

    out << block.size();    // Inicio del bloque 1/5    quint
    out << relative_dir;    // Path relativo 2/5    qstring
    out << num_files;       // Número de archivos 3/5   int

    for (int q = 0; q < num_files ; q++){
        QFile file_(dir.filePath(list_files[q]));
        QFileInfo f(file_);

        //QFile fi(“C:/Files/Input.jpg”); // this is your input file
        //QFile fo(“C:/Files/output.compressed”); // this is your compressed output file
/*
        if(file_.open(QFile::ReadOnly))
        {
        QByteArray cpy,cpy_umc;
        int compress_level = 9; // compression level
        cpy = (qCompress(file_.readAll(), compress_level)); // read input, compress and write to output is a single line of code
        qDebug() << "nombre archivo --> " << f.fileName();

        out << f.fileName(); //Nombre archivo 4/5 QFile
        ui->listWidget->addItem(f.fileName());
        out << cpy; //Archivo 5/5   qbytearray
        byte += f.size();
        }
*/
        if (file_.open(QIODevice::ReadOnly)){
            QByteArray cpy;
            cpy = file_.readAll();
            out << f.fileName(); //Nombre archivo 4/5 QFile
            ui->listWidget->addItem(f.fileName());
            out << cpy; //Archivo 5/5   qbytearray
            byte += file_.size();
        }

    }


    out.device()->seek(0);
    out << block.size();
    sender_block_ += byte;
    if (mSocket->readBufferSize() > mSocket->bytesToWrite()){
        mSocket->write(block);
        qDebug() << block.size() << "envaidos";
        ui->progressBar->setValue(sender_block_ / size_files_ *100);
        if (!mSocket->bytesToWrite())
            mSocket->waitForBytesWritten(-1);
        mSocket->flush();
        if (num_dir != 0){
            for (int q = 0; q < num_dir ; q++){
                do_block(relative_dir, dir.filePath(list_dir[q]));
            }
        }
    }
    /*else {
        do_block(relative_dir, dir);
    }*/
}

void Cliente::write_data(QByteArray buffer_llegada)
{
    QDataStream read(&buffer_llegada,QIODevice::ReadOnly);


    int bytesize;
    QString bit;
    int num;
    QString name;
    QByteArray contain;

    read >> bytesize;

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
        ui->listWidget->addItem(name);
        QFile file(dir.absoluteFilePath(name));
        if (file.open(QIODevice::WriteOnly)){
            //QByteArray un_cmp = qUncompress(contain);
            file.write(contain);
            file.close();
        }
    }
    ui->progressBar->setValue(sender_block_ / size_files_ *100);
}

void Cliente::send_all_block()
{
        qDebug() << "Hay clientes";

        QDataStream read(mSocket->read(mSocket->bytesAvailable()));
        quint64 data;
        int client;
        QString espera;
        read >> client;
        if (client >= ui->maxclient->value()){
            ui->listWidget->clear();
            espera =  "Hay suficientes clientes, empezando a enviar:";
            ui->listWidget->addItem(espera);
            read >> client;
            qDebug() << sizeof(read) << "hay " << client << " clientes";

            QDir drs(data_.second);
            do_block("",drs);
        }
        else {
            QString espera = "Solo hay ";
            espera+= QString::number(client);
            ui->listWidget->addItem(espera);
        }
}

void Cliente::on_pushFolder_clicked()
{
     data_.second = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
     ui->Direrctorio->setText(data_.second);
     ui->conectar->setEnabled(true);
}
