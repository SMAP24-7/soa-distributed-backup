#ifndef SELECT_MODE_CLIENT_H
#define SELECT_MODE_CLIENT_H

#include <QFileDialog>
#include <QDebug>
#include <QPair>
#include <QWidget>
#include <QDialog>

namespace Ui {
class select_mode_client;
}

class select_mode_client : public QWidget
{
    Q_OBJECT

public:
    explicit select_mode_client(QWidget *parent = 0);
    ~select_mode_client();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Ok_clicked();

private:
    Ui::select_mode_client *ui;
    QPair<QString,QString> data_;

};

#endif // SELECT_MODE_CLIENT_H
