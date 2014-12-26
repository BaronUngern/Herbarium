#ifndef ADDITION_H
#define ADDITION_H

#include <QDialog>

namespace Ui {
class addition;
}

class addition : public QDialog
{
    Q_OBJECT

public:
    explicit addition(QWidget *parent = 0);
    ~addition();

private:
    Ui::addition *ui;
};

#endif // ADDITION_H
