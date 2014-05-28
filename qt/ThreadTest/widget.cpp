#include "widget.h"
#include "ui_widget.h"
#include "async.h"
#include <QThread>

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
    QCoreApplication::processEvents();

    QThread* thread = new QThread();
    Async* async = new Async(this, &canContinue);

    async->moveToThread(thread);
    connect(async, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), async, SLOT(process()));
    connect(async, SIGNAL(finished()), thread, SLOT(quit()));
    connect(async, SIGNAL(finished()), async, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(enableSyncButton()));

    connect(async, SIGNAL(print(const char*)), this, SLOT(print(const char*)));

    thread->start();
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
