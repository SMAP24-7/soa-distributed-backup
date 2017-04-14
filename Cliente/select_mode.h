#ifndef SELECT_MODE_H
#define SELECT_MODE_H

#include <QFileDialog>
#include <QWidget>

namespace Ui {
class select_mode;
}

class select_mode : public QWidget
{
    Q_OBJECT

public:
    explicit select_mode(QWidget *parent = 0);
    ~select_mode();

private:
    Ui::select_mode *ui;
};

#endif // SELECT_MODE_H
