#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QScrollBar>
#include <QDebug>

bool g_quit = false;
QWaitCondition g_canContinue;
QMutex g_mutex;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    int result = a.exec();
    qDebug() << "Exit app.";
    return result;
}
