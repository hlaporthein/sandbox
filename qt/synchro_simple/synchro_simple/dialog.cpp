#include <QtWidgets>
#include <QApplication>
#include <QMetaType>
#include <QtConcurrent/QtConcurrent>

#include "dialog.h"
#include "ui_dialog.h"
#include "worker.h"
extern "C" {
#include "../../../copy_struct_test/synchro.h"
}

Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog),
    tray(new QSystemTrayIcon(this))
{
    qRegisterMetaType<QTextCursor>("QTextCursor");
    ui->setupUi(this);
    createTrayIcon();
    ui->progressBar->setVisible(false);
    ui->remainingLabel->setVisible(false);
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
    ui->remainingLabel->setText("Remaining time: TODO");
    //start_t = clock();
    QCoreApplication::processEvents();

    Worker* worker = new Worker(this, &canContinue, &mutex);

    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this, SLOT(finishedProcess()));
    connect(worker, SIGNAL(print(const char*)), this, SLOT(print(const char*)));
    connect(worker, SIGNAL(progressBar(int,int)), this, SLOT(progressBar(int, int)));

    QtConcurrent::run(worker, &Worker::process);
}

void Dialog::progressBar(int total, int val) {
    mutex.lock();
    ui->progressBar->setRange(0, total);
    ui->progressBar->setValue(val);

    //double speed = val / (clock() - start_t);
    int remaining = 0;
    ui->remainingLabel->setText("Remaining: " + remaining);
    //qDebug() << "2. ui: about to wakeall.";
    canContinue.wakeAll();
    //qDebug() << "3. ui: just waked all.";
    mutex.unlock();
}

void Dialog::finishedProcess() {
    enableProcess(false);
}

void Dialog::on_abortButton_clicked()
{
    ui->abortButton->setEnabled(false);
    set_abort();
}

void Dialog::enableProcess(bool enabled) {
    ui->syncButton->setEnabled(!enabled);
    ui->abortButton->setEnabled(enabled);
    ui->remainingLabel->setText("");
    ui->remainingLabel->setVisible(enabled);
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
    tray->setToolTip("Synchro");
    tray->show();

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
//    e->ignore();
//    hide();
}

void Dialog::quit() {
    QApplication::quit();
}

void Dialog::print(const char* buf) {
    mutex.lock();
    ui->traceTextEdit->insertPlainText(buf);
    ui->traceTextEdit->moveCursor(QTextCursor::End);
    //qDebug() << "2. ui: about to wakeall.";
    canContinue.wakeAll();
    //qDebug() << "3. ui: just waked all.";
    mutex.unlock();
}
