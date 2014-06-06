#include "worker.h"
#include "dialog.h"
#include "ui_dialog.h"

#include <QThread>
#include <QtDebug>
#include <QDir>

extern "C" {
#include "../../../synchro_test/synchro.h"
}

Worker *g_worker = NULL;

void worker_print(const char* buf) {
    g_worker->printMsg(buf);
}

void worker_progress_value(int i) {
    g_worker->sendValue(i);
}

Worker::Worker(Dialog* d) : dialog(d) {
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

    set_max_op(settings.value(CONF_MAX_OP, CONF_DEF_MAX_OP).toInt());
    set_print(worker_print);
    set_progress_value(worker_progress_value);
    set_progress_min_delay(0);
    set_temp_dir(QDir::tempPath().toLocal8Bit().data());
    reset_abort();

    int status = sync_dir_build_cmd(srcBuf, dstBuf, 0);

    progressTotal = g_total_step;

    status = run_file();

    switch(status) {
        case 0:
            strncpy(buf, "Finished with success.\n", 1024);
            break;
        case 1:
            strncpy(buf, "Aborted.\n", 1024);
            break;
        case 2:
            strncpy(buf, "Finished partially with success.\n", 1024);
            break;
        case -1:
            strncpy(buf, "Error.\n", 1024);
            break;
        default:
            snprintf(buf, 1024, "Error, staus unknowns: %d.\n", status);
    }
    printMsg(buf);

//cleanup:
    emit finished();
}

void Worker::printMsg(const char* buf) {
    if (g_quit) {
        return;
    }
    g_mutex.lock();
    emit print(buf);
    //qDebug() << "1. thread->about to wait.";
    g_canContinue.wait(&g_mutex);
    //qDebug() << "4. thread->just waked up.";
    g_mutex.unlock();
}

void Worker::sendValue(const int val) {
    if (g_quit) {
        return;
    }
    g_mutex.lock();
    emit progressBar(progressTotal, val);
    //qDebug() << "1. send value thread->about to wait.";
    g_canContinue.wait(&g_mutex);
    //qDebug() << "4. send value thread->just waked up.";
    g_mutex.unlock();
}
