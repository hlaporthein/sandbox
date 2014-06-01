#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

#include "dialog.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(Dialog* w, QWaitCondition* c, QMutex* m);
    void printMsg(const char* buf);

signals:
    void finished();
    void error(QString err);
    void print(const char* buf);

public slots:
    void process();

private:
    QMutex* mutex;
    Dialog* dialog;
    QWaitCondition* canContinue;

};

#endif // WORKER_H
