#include "worker.h"
#include "dialog.h"
#include "ui_dialog.h"

#include <QThread>
#include <QtDebug>

extern "C" {
#include "../../../copy_struct_test/synchro.h"
}

Worker *g_worker = NULL;

void worker_print(const char* buf) {
    g_worker->printMsg(buf);
}

Worker::Worker(Dialog* d, QWaitCondition* c, QMutex* m) :
    dialog(d), canContinue(c), mutex(m)
{
}

void Worker::process() {
    //QThread::msleep(1000);
    char srcBuf[PATH_SIZE];
    char dstBuf[PATH_SIZE];
    char buf[PATH_SIZE];
    char* str = dialog->getUi()->srcLineEdit->text().toLocal8Bit().data();
    strncpy(srcBuf, str, PATH_SIZE);
    char* dst = dialog->getUi()->dstLineEdit->text().toLocal8Bit().data();
    strncpy(dstBuf, dst, PATH_SIZE);

    printMsg(srcBuf);
    printMsg("\n");
    printMsg(dstBuf);
    printMsg("\n");
    g_worker = this;
    set_print(worker_print);
    sync_dir(srcBuf, dstBuf);

    strncpy(buf, "Finished.\n", 1024);
    printMsg(buf);

    emit finished();
}

void Worker::printMsg(const char* buf) {
    mutex->lock();
    emit print(buf);
    //qDebug() << "1. thread->about to wait.";
    canContinue->wait(mutex);
    //qDebug() << "4. thread->just waked up.";
    mutex->unlock();
}
