#include "widget.h"
#include "ui_widget.h"
#include "async.h"
#include <QThread>
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

void Widget::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);

    Async* async = new Async(this);

    connect(async, SIGNAL(finished()), async, SLOT(deleteLater()));
    connect(async, SIGNAL(finished()), this, SLOT(enableSyncButton()));
    connect(async, SIGNAL(print(const char*)), this, SLOT(print(const char*)));

    QtConcurrent::run(async, &Async::process());
}

void Widget::enableSyncButton() {
    ui->pushButton->setEnabled(true);
    QCoreApplication::processEvents();
}

void Widget::print(const char* buf) {
    ui->textEdit->insertPlainText(buf);
    canContinue.wakeAll();
}

void Widget::errorString(QString str) {
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertHtml("<font color=\"red\">" + str + "</font>");
}
