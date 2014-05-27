#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H

#include <QObject>
#include "dialog.h"
class Synchronizer : public QObject
{
    Q_OBJECT
public:
    explicit Synchronizer(Dialog *d);
    ~Synchronizer();

signals:
    void finished();
    void error(QString err);

public slots:
    void process();

private:
    Dialog* dialog;
};

#endif // SYNCHRONIZER_H
