#include "widget.h"
#include "ui_widget.h"
#include "worker.h"
#include <QtConcurrent/QtConcurrent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked() {
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    worker = new Worker();
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this, SLOT(on_stopButton_clicked()));
    connect(worker, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));

    QtConcurrent::run(worker, &Worker::process);
}

void Widget::on_stopButton_clicked() {
    worker->cancel = true;
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->progressBar->setValue(0);
}
