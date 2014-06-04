#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QScrollBar>
#include <QDebug>

#include "version.h"

bool g_quit = false;
QWaitCondition g_canContinue;
QMutex g_mutex;

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QApplication::setApplicationName(VER_PRODUCTNAME_STR);

    QApplication a(argc, argv);
    Dialog w;
    w.show();
    int result = a.exec();
    qDebug() << "Exit app.";
    return result;
}
