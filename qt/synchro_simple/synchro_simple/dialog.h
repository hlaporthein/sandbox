#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QWaitCondition>
#include <QMutex>
#include <QSettings>
#include <QTimer>
#include <QTime>

#include "options.h"
#include "settings.h"
#include "../../../synchro_test/synchro.h"

extern bool g_quit;
extern QWaitCondition g_canContinue;
extern QMutex g_mutex;

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void closeEvent(QCloseEvent * e);
    Ui::Dialog* getUi() { return ui; }
    void setBackgroundMode(bool isBackground);

private slots:
    void on_srcBrowseButton_clicked();
    void on_dstBrowseButton_clicked();
    void on_syncButton_clicked();
    void on_abortButton_clicked();
    void on_moreButton_clicked();
    void on_clearButton_clicked();
    void showNormalOnDblClick(QSystemTrayIcon::ActivationReason reason);
    void hideShow();
    void quit();
    void errorString(QString str);
    void print(const char* buf);
    void finishedProcess();
    void progressBar(int64 total, int64 val);
    void backgroundSync();
private:
    Ui::Dialog *ui;
    QSystemTrayIcon* tray;
    QSettings settings;
    Options* o;
    QTimer timer;
    QTime time;

    void createTrayIcon();
    void enableProcess(bool enabled);
    void getRemainingStr(char *buf, int size, int sec);

};

#endif // DIALOG_H
