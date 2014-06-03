#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "worker.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();

private:
    Ui::Widget *ui;
    Worker* worker;
};

#endif // WIDGET_H
