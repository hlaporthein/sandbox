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

void manageProcessPriority() {
#ifdef __WIN32__
    QSettings settings;
    BOOL res = SetPriorityClass(GetCurrentProcess(), settings.value(CONF_PRIORITY, CONF_DEF_PRIORITY).toInt());
    if (!res) {
        SetPriorityClass(GetCurrentProcess(), CONF_DEF_PRIORITY);
        settings.setValue(CONF_PRIORITY, CONF_DEF_PRIORITY);
    }
#endif
}

void loadDefaultConf() {
    QSettings settings;
    if (settings.value(CONF_EXISTING_CONF, false).toBool()) {
        return;
    }

    settings.setValue(CONF_EXISTING_CONF, true);

    int i = 0;
    settings.beginWriteArray(CONF_FILTERS);
    settings.remove("");
    settings.setArrayIndex(i);
    settings.setValue(CONF_FILTERS_IS_DIR, false);
    settings.setValue(CONF_FILTERS_LABEL, "MSOffice temp file");
    settings.setValue(CONF_FILTERS_VALUE, "^~\\$");
    i++;
    settings.endArray();
}

int main(int argc, char *argv[])
{
    qDebug() << "Start app.";
    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QApplication::setApplicationName(VER_PRODUCTNAME_STR);

    manageProcessPriority();

    loadDefaultConf();

    QApplication a(argc, argv);
    Dialog w;
    w.show();
    int result = a.exec();
    qDebug() << "Exit app.";
    return result;
}
