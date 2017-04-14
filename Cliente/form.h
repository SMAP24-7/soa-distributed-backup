#ifndef FORM_H
#define FORM_H

#include <QDialog>
#include <QFileDialog>
#include <QPair>
#include <QThreadPool>
#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    QPair<QString,QString> data_;

private slots:

    void on_origen_clicked();

    void on_destino_clicked();

    void on_ok_clicked();

private:
    Ui::Form *ui;


};

#endif // FORM_H
