#include "worker.h"
#include <windows.h>

#include <QThread>
#include <QtDebug>


Worker::Worker() : cancel(false) {
}

void Worker::process() {
    for (int i = 0; i < 100; i++) {
        if (cancel) {
            break;
        }
        Sleep(200);
        emit progress(i);
    }
    emit finished();
}
