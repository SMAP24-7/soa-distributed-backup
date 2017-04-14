#ifndef WIDGET_H
#define WIDGET_H

#include "ui_widget.h"
#include "server.h"

#include <QMessageBox>
#include <QTextStream>
#include <QWidget>

namespace Ui {
class Widget;
}

class LocalServer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_iniciar_clicked();

    void on_enviar_clicked();

    void on_cerrar_clicked();

private:
    Ui::Widget *ui;
    LocalServer *mServer;
};

#endif // WIDGET_H
