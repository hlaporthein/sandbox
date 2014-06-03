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

void worker_progress_value(int i) {
    g_worker->sendValue(i);
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

    g_worker = this;

    set_print(worker_print);
    set_progress_value(worker_progress_value);
    set_progress_min_delay(0);

    set_mode(PREVIEW_MODE);
    int status = sync_dir(srcBuf, dstBuf);

    progressTotal = get_total_step();

    set_mode(REAL_MODE);
    status = sync_dir(srcBuf, dstBuf);

    reset_abort();

    if (status == 0) {
        strncpy(buf, "Finished with success.\n", 1024);
    } else {
        strncpy(buf, "Aborted.\n", 1024);
    }
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

void Worker::sendValue(const int val) {
    mutex->lock();
    emit progressBar(progressTotal, val);
    //qDebug() << "1. thread->about to wait.";
    canContinue->wait(mutex);
    //qDebug() << "4. thread->just waked up.";
    mutex->unlock();
}
