#ifndef ASYNC_H
#define ASYNC_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "widget.h"

class Async : public QObject
{
    Q_OBJECT
public:
    explicit Async(Widget* w, QWaitCondition* c);

signals:
    void finished();
    void error(QString err);
    void print(const char* buf);

public slots:
    void process();

private:
    Widget* widget;
    QMutex mutex;
    QWaitCondition* canContinue;

    void printMsg(const char* buf);

};

#endif // ASYNC_H
