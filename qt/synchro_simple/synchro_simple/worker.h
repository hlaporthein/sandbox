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
    explicit Worker(Dialog* w);
    void printMsg(const char* buf);
    void sendValue(const int val);

signals:
    void finished();
    void error(QString err);
    void print(const char* buf);
    void progressBar(int total, int val);

public slots:
    void process();

private:
    Dialog* dialog;
    int progressTotal;
};

#endif // WORKER_H
