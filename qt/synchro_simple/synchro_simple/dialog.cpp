#include <QtWidgets>
#include <QApplication>
#include <QMetaType>

#include "dialog.h"
#include "ui_dialog.h"
#include "synchronizer.h"
extern "C" {
    #include "../../../copy_struct_test/synchro.h"
}

extern Ui::Dialog* g_uip;

Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog),
    tray(new QSystemTrayIcon(this))
{
    qRegisterMetaType<QTextCursor>("QTextCursor");
    ui->setupUi(this);
    g_uip = ui;
    createTrayIcon();
}

Dialog::~Dialog()
{
    delete ui;
    delete tray;
}

void Dialog::on_srcBrowseButton_clicked()
{
    QString src = ui->srcLineEdit->text();
    if (src.isEmpty()) {
        src = "/home";
    }
    QString dir = QFileDialog::getExistingDirectory(this,
                        "Choose directory to synchonise",
                        src,
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->srcLineEdit->setText(dir);
    }
}

void Dialog::on_dstBrowseButton_clicked()
{
    QString dst = ui->dstLineEdit->text();
    if (dst.isEmpty()) {
        dst = "/home";
    }
    QString dir = QFileDialog::getExistingDirectory(this,
                        "Choose destination directory",
                        dst,
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->dstLineEdit->setText(dir);
    }
}

void Dialog::on_syncButton_clicked()
{
    QString src = ui->srcLineEdit->text();
    QString dst = ui->dstLineEdit->text();
    if (src.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("You have to choose a folder to synchronize");
        msgBox.exec();
        return;
    }
    if (dst.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("You have to choose a destination folder");
        msgBox.exec();
        return;
    }

    //ui->traceTextEdit->clear();

    QByteArray src_b = src.toLocal8Bit();
    QByteArray dst_b = dst.toLocal8Bit();

    //sync_dir(src_b.data(), dst_b.data());
    ui->syncButton->setEnabled(false);

    QThread* thread = new QThread;
    Synchronizer* synchronizer = new Synchronizer(this);
    synchronizer->moveToThread(thread);
    connect(synchronizer, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), synchronizer, SLOT(process()));
    connect(synchronizer, SIGNAL(finished()), thread, SLOT(quit()));
    connect(synchronizer, SIGNAL(finished()), synchronizer, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(enableSyncButton()));
    thread->start();
}

void Dialog::enableSyncButton() {
    ui->syncButton->setEnabled(true);
}

void Dialog::errorString(QString str) {
    ui->traceTextEdit->moveCursor(QTextCursor::End);
    ui->traceTextEdit->insertHtml("<font color=\"red\">" + str + "</font>");
}

void Dialog::createTrayIcon() {
    QMenu* trayMenu = new QMenu(this);

    QAction* hideShowAction = new QAction("&Hide/Show", this);
    connect(hideShowAction, SIGNAL(triggered()), this, SLOT(hideShow()));
    trayMenu->addAction(hideShowAction);

    trayMenu->addSeparator();
    QAction* quitAction = new QAction("&Quit", this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    trayMenu->addAction(quitAction);

    tray->setContextMenu(trayMenu);
    tray->setIcon(QIcon(":/icon/ocp_icon.png"));
    tray->setToolTip("Synchro");
    tray->show();

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(showNormalOnDblClick(QSystemTrayIcon::ActivationReason)));
}

void Dialog::showNormalOnDblClick(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        showNormal();
    }
}

void Dialog::hideShow() {
    if (isVisible()) {
        hide();
    } else {
        showNormal();
    }
}

void Dialog::closeEvent(QCloseEvent * e) {
//    e->ignore();
//    hide();
}

void Dialog::quit() {
    QApplication::quit();
}
