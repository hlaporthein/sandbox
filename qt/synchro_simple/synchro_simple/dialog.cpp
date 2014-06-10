#include <QtWidgets>
#include <QApplication>
#include <QMetaType>
#include <QtConcurrent/QtConcurrent>
#include <ctime>
#include <sstream>

#include "dialog.h"
#include "ui_dialog.h"
#include "worker.h"
#include "options.h"
#include "version.h"

#include "../../../synchro_test/synchro.h"

Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog),
    tray(new QSystemTrayIcon(this)),
    o(new Options(this))
{
    qRegisterMetaType<QTextCursor>("QTextCursor");
    ui->setupUi(this);
    createTrayIcon();
    setBackgroundMode(settings.value(CONF_USE_PERIOD, CONF_DEF_USE_PERIOD).toBool());
    enableProcess(false);

    ui->srcLineEdit->setText(settings.value(CONF_SRC_DIR, "").toString());
    ui->dstLineEdit->setText(settings.value(CONF_DST_DIR, "").toString());

    connect(&timer, SIGNAL(timeout()), this, SLOT(backgroundSync()));
}

Dialog::~Dialog() {
    delete ui;
    delete tray;
    delete o;
}

void Dialog::on_srcBrowseButton_clicked() {
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
        settings.setValue(CONF_SRC_DIR, dir);
    }
}

void Dialog::on_dstBrowseButton_clicked() {
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
        settings.setValue(CONF_DST_DIR, dir);
    }
}

void Dialog::on_syncButton_clicked() {
    time.restart();
    ui->traceTextEdit->clear();

    // Checking
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

    // Starting synchronizing (long asynchrone operation)
    enableProcess(true);
    QCoreApplication::processEvents();

    Worker* worker = new Worker(this);

    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this, SLOT(finishedProcess()));
    connect(worker, SIGNAL(print(const char*)), this, SLOT(print(const char*)));
    connect(worker, SIGNAL(progressBar(int,int)), this, SLOT(progressBar(int, int)));

    QtConcurrent::run(worker, &Worker::process);
}

void Dialog::progressBar(int total, int val) {
    g_mutex.lock();
    ui->progressBar->setRange(0, total);
    ui->progressBar->setValue(val);

    int elapsed = time.elapsed() / 1000;
    double speed = val / ((double) elapsed);
    int remaining = (total - val) / speed;

    char buf[BUFFER_SIZE];
    snprintf(buf, BUFFER_SIZE, "Remaining: %ds", remaining);
    ui->remainingLabel->setText(buf);
    ui->remainingLabel->repaint();

    snprintf(buf, BUFFER_SIZE, "Sync. data: %d of %d KB", val / 1024, total / 1024);
    ui->bytesDoneLabel->setText(buf);
    ui->bytesDoneLabel->repaint();

    //qDebug() << "2. ui: about to wakeall.";
    g_canContinue.wakeAll();
    //qDebug() << "3. ui: just waked all.";
    g_mutex.unlock();
}

void Dialog::finishedProcess() {
    enableProcess(false);
}

void Dialog::on_abortButton_clicked() {
    ui->abortButton->setEnabled(false);
    set_abort();
}

void Dialog::enableProcess(bool enabled) {
    ui->syncButton->setEnabled(!enabled);
    ui->abortButton->setEnabled(enabled);
    ui->remainingLabel->setText("Prepare to sync...");
    ui->remainingLabel->setVisible(enabled);
    ui->bytesDoneLabel->setText("");
    ui->bytesDoneLabel->setVisible(enabled);
    ui->progressBar->setRange(0, 0);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(enabled);
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
    tray->setToolTip(VER_PRODUCTNAME_STR);

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(showNormalOnDblClick(QSystemTrayIcon::ActivationReason)));
}

void Dialog::showNormalOnDblClick(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        setWindowState(Qt::WindowActive);
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
    if (settings.value(CONF_USE_PERIOD, CONF_DEF_USE_PERIOD).toBool()) {
        e->ignore();
        hide();
    } else {
        quit();
    }
}

void Dialog::quit() {
    g_mutex.lock();
    g_quit = true;
    set_abort();
    g_canContinue.wakeAll();
    QApplication::quit();
    g_mutex.unlock();
}

void Dialog::print(const char* buf) {
    g_mutex.lock();
    ui->traceTextEdit->insertPlainText(buf);
    ui->traceTextEdit->moveCursor(QTextCursor::End);
    //qDebug() << "2. ui: about to wakeall.";
    g_canContinue.wakeAll();
    //qDebug() << "3. ui: just waked all.";
    g_mutex.unlock();
}

void Dialog::on_moreButton_clicked() {
    o->show();
}

void Dialog::on_clearButton_clicked() {
    ui->traceTextEdit->clear();
}

void Dialog::setBackgroundMode(bool isBackground) {
    tray->setVisible(isBackground);
    if (isBackground) {
        timer.start(settings.value(CONF_PERIOD, CONF_DEF_PERIOD).toInt() * 1000);
        time.start();
    } else {
        timer.stop();
    }
}

void Dialog::backgroundSync() {
    if (ui->syncButton->isEnabled()) {
        on_syncButton_clicked();
    }
}
