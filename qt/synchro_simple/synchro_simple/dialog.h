#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QWaitCondition>
#include <QMutex>

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

private slots:
    void on_srcBrowseButton_clicked();
    void on_dstBrowseButton_clicked();
    void on_syncButton_clicked();
    void on_abortButton_clicked();
    void showNormalOnDblClick(QSystemTrayIcon::ActivationReason reason);
    void hideShow();
    void quit();
    void errorString(QString str);
    void print(const char* buf);
    void finishedProcess();
    void progressBar(int total, int val);


private:
    Ui::Dialog *ui;
    QSystemTrayIcon* tray;
    unsigned int start_t;

    void createTrayIcon();
    void enableProcess(bool enabled);


};

#endif // DIALOG_H
