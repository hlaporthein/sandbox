#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>

void my_print(const char* buf);

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
    Ui::Dialog *ui;

private slots:
    void on_srcBrowseButton_clicked();
    void on_dstBrowseButton_clicked();
    void on_syncButton_clicked();
    void showNormalOnDblClick(QSystemTrayIcon::ActivationReason reason);
    void hideShow();
    void quit();
    void errorString(QString str);
    void enableSyncButton();

private:
    QSystemTrayIcon* tray;

    void createTrayIcon();
};

#endif // DIALOG_H
