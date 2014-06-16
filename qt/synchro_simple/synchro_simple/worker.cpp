#include "worker.h"
#include "dialog.h"
#include "ui_dialog.h"

#include <QThread>
#include <QtDebug>
#include <QDir>

#include "../../../synchro_test/synchro.h"

Worker *g_worker = NULL;

void worker_print(const char* buf) {
    g_worker->printMsg(buf);
}

void worker_progress_value(int64 i) {
    g_worker->sendValue(i);
}

Worker::Worker(Dialog* d) :
    dialog(d),
    filters(NULL),
    filters_length(0) {
}

void Worker::process() {
    //QThread::msleep(1000);
    char srcBuf[PATH_SIZE];
    char dstBuf[PATH_SIZE];
    char tmpBuf[PATH_SIZE];
    char buf[PATH_SIZE];
    char* str = settings.value(CONF_SRC_DIR).toString().toUtf8().data();
    strncpy(srcBuf, str, PATH_SIZE);
    char* dst = settings.value(CONF_DST_DIR).toString().toUtf8().data();
    strncpy(dstBuf, dst, PATH_SIZE);
    strncpy(tmpBuf, QDir::tempPath().toUtf8().data(), PATH_SIZE);

    g_worker = this;

    reset_abort();
    set_max_op(settings.value(CONF_MAX_OP, CONF_DEF_MAX_OP).toInt());
    set_print(worker_print);
    set_progress_value(worker_progress_value);
    set_progress_min_delay(
                settings.value(CONF_PROGRESS_REFRESH_RATE, CONF_DEF_PROGRESS_REFRESH_RATE).toInt());
    set_temp_dir(tmpBuf);
    qDebug() << "tmp_dir=" << tmpBuf;

    setFilters();
    int status = sync_dir_build_cmd(srcBuf, dstBuf, 0);
    freeFilters();

    progressTotal = g_total_step;

    status = run_file();

    switch (status) {
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

void Worker::setFilters() {
    filters_length = settings.beginReadArray(CONF_FILTERS);
    filters = (filter_t*) malloc(sizeof(filter_t) * filters_length);
    for (int i = 0; i < filters_length; i++) {
        settings.setArrayIndex(i);
        strncpy(filters[i].label,
                settings.value(CONF_FILTERS_LABEL, "").toString().toUtf8().data(), BUFFER_SIZE);
        strncpy(filters[i].filter,
                settings.value(CONF_FILTERS_VALUE, "").toString().toUtf8().data(), BUFFER_SIZE);
        filters[i].is_dir = (settings.value(CONF_FILTERS_IS_DIR).toBool())? TRUE : FALSE;
    }
    settings.endArray();
    set_filter(filters_length, filters);
}

void Worker::freeFilters() {
    free_filter();
    free(filters);
    filters = NULL;
    filters_length = 0;
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

void Worker::sendValue(const int64 val) {
    if (g_quit) {
        return;
    }
    g_mutex.lock();
    //qDebug() << "progressTotal=" << progressTotal << "   val=" << val;
    emit progressBar(progressTotal, val);
    //qDebug() << "1. send value thread->about to wait.";
    g_canContinue.wait(&g_mutex);
    //qDebug() << "4. send value thread->just waked up.";
    g_mutex.unlock();
}
