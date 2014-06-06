#include <QDebug>

#include "options.h"
#include "dialog.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    ui->maxOpLineEdit->setValidator(new QIntValidator(0, 1000000, this));
}

Options::~Options() {
    delete ui;
}

void Options::load() {
    qDebug() << "load";
    ui->maxOpLineEdit->setText(settings.value(CONF_MAX_OP, CONF_DEF_MAX_OP).toString());
}

void Options::save() {
    qDebug() << "save";
    settings.setValue(CONF_MAX_OP, ui->maxOpLineEdit->text().toInt());
}

void Options::reset() {
    qDebug() << "reset";
    ui->maxOpLineEdit->setText(QString::number(CONF_DEF_MAX_OP));
}

void Options::show() {
    qDebug() << "show";
    load();
    restoreGeometry(settings.value(CONF_OPTIONS_GEOMETRY).toByteArray());
    QDialog::show();
}

void Options::accept() {
    qDebug() << "accept";
    settings.setValue(CONF_OPTIONS_GEOMETRY, saveGeometry());
    QDialog::accept();
}

void Options::reject() {
    qDebug() << "reject";
    settings.setValue(CONF_OPTIONS_GEOMETRY, saveGeometry());
    QDialog::reject();
}

void Options::on_buttonBox_clicked(QAbstractButton *button) {
    switch(ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Reset:
        reset();
        break;
    case QDialogButtonBox::Save:
        save();
        break;
    case QDialogButtonBox::Cancel:
        break;
    default:
        break;
    }
}
