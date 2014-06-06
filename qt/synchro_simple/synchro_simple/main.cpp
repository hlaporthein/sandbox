#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QScrollBar>
#include <QDebug>

#ifdef __WIN32__
#include <windows.h>
#endif

#include "version.h"

bool g_quit = false;
QWaitCondition g_canContinue;
QMutex g_mutex;

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QApplication::setApplicationName(VER_PRODUCTNAME_STR);

#ifdef __WIN32__
    QSettings settings;
    BOOL res = SetPriorityClass(GetCurrentProcess(), settings.value(CONF_PRIORITY, CONF_DEF_PRIORITY).toInt());
    if (!res) {
        SetPriorityClass(GetCurrentProcess(), CONF_DEF_PRIORITY);
        settings.setValue(CONF_PRIORITY, CONF_DEF_PRIORITY);
    }
#endif

    QApplication a(argc, argv);
    Dialog w;
    w.show();
    int result = a.exec();
    qDebug() << "Exit app.";
    return result;
}
