#include "synchronizer.h"
#include "dialog.h"
#include "ui_dialog.h"
#include <windows.h>
#include <QDebug>
#include <QScrollBar>
extern "C" {
    #include "../../../copy_struct_test/synchro.h"
}

extern Ui::Dialog* g_uip;

Synchronizer::Synchronizer(Dialog* d) :
    dialog(d)
{
}

void Synchronizer::printMsg(const char *buf) {
    my_print(buf);
}

Synchronizer::~Synchronizer() {
    // free resources
}

void Synchronizer::process() {
    QByteArray ba = dialog->ui->srcLineEdit->text().toLocal8Bit();
    char* src = dialog->ui->srcLineEdit->text().toLocal8Bit().data();
    char buf[1024];
//    snprintf(buf, 1024, "|%s|", src);
    strncpy(buf, src, 1024);
    qDebug() << ba.length();
    qDebug() << buf;
    printMsg("src=");
    printMsg(buf);
    printMsg("\n");
    QCoreApplication::processEvents();

    char* dst = dialog->ui->dstLineEdit->text().toLocal8Bit().data();
    strncpy(buf, dst, 1024);
    printMsg("dst=");
    printMsg(buf);
    printMsg("\n");

//    sync_dir(src, dst);
//    Sleep(5000);
    emit finished();
}

//void Synchronizer::finished() {}

//void Synchronizer::error(QString err) {}
