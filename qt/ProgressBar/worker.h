#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker();
    void printMsg(const char* buf);
    bool cancel;

signals:
    void finished();
    void progress(int progression);

public slots:
    void process();

};

#endif // WORKER_H
