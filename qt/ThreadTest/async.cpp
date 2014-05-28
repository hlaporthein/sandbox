#include "async.h"
#include "widget.h"
#include "ui_widget.h"
#include <windows.h>

Async::Async(Widget* w, QWaitCondition* c) : widget(w), canContinue(c)
{
}

void Async::process() {
    Sleep(1000);
    char buf[1024];

    char* str = widget->getUi()->lineEdit->text().toLocal8Bit().data();
    strncpy(buf, str, 1024);

    printMsg(buf);
    printMsg("\n");

    strncpy(buf, "plouf...\n", 1024);
    printMsg(buf);

    emit finished();
}

void Async::printMsg(const char* buf) {
    mutex.lock();
    emit print(buf);
    canContinue->wait(&mutex);
    mutex.unlock();
}
