#include <QFileDialog>
#include <QMessageBox>

#include "dialog.h"
#include "ui_dialog.h"
extern "C" {
    #include "../../../copy_struct_test/synchro.h"
}

extern Ui::Dialog* g_uip;

Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    g_uip = ui;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_srcBrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                        "Choose directory to synchonise",
                        "/home",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->srcLineEdit->setText(dir);
}

void Dialog::on_dstBrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                        "Choose destination directory",
                        "/home",
                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->dstLineEdit->setText(dir);
}

void print() {

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
    QByteArray src_b = src.toLocal8Bit();
    QByteArray dst_b = dst.toLocal8Bit();

    sync_dir(src_b.data(), dst_b.data());
}
