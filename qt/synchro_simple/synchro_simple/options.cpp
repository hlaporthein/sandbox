#include <QDebug>

#include "options.h"
#include "dialog.h"
#include "ui_options.h"

#ifdef __WIN32__
#include "windows.h"
#endif

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    ui->maxOpLineEdit->setValidator(new QIntValidator(0, 1000000, this));
    ui->periodLineEdit->setValidator(new QIntValidator(0, 3600*24, this));
    ui->progressRefreshrateLineEdit->setValidator(new QIntValidator(0, 5000, this));

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->resizeSection(0, 20);

#ifdef __WIN32__
    ui->priorityComboBox->addItem("Low", QVariant(IDLE_PRIORITY_CLASS));
    ui->priorityComboBox->addItem("Below Normal", QVariant(BELOW_NORMAL_PRIORITY_CLASS));
    ui->priorityComboBox->addItem("Normal", QVariant(NORMAL_PRIORITY_CLASS));
    ui->priorityComboBox->addItem("Above Normal", QVariant(ABOVE_NORMAL_PRIORITY_CLASS));
    ui->priorityComboBox->addItem("High", QVariant(HIGH_PRIORITY_CLASS));
    ui->priorityComboBox->addItem("Real Time", QVariant(REALTIME_PRIORITY_CLASS));
#else
    ui->priorityComboBox->setEnabled(false);
#endif
}

Options::~Options() {
    delete ui;
}

void Options::load() {
    qDebug() << "load";
    ui->maxOpLineEdit->setText(settings.value(CONF_MAX_OP, CONF_DEF_MAX_OP).toString());

    bool usePeriod = settings.value(CONF_USE_PERIOD, CONF_DEF_USE_PERIOD).toBool();
    ui->periodLineEdit->setText(settings.value(CONF_PERIOD, CONF_DEF_PERIOD).toString());
    ui->periodLineEdit->setEnabled(usePeriod);
    ui->periodCheckBox->setChecked(usePeriod);

    ui->progressRefreshrateLineEdit->setText(
                settings.value(CONF_PROGRESS_REFRESH_RATE, CONF_DEF_PROGRESS_REFRESH_RATE).toString());

    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }
    int size = settings.beginReadArray(CONF_FILTERS);
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        ui->tableWidget->insertRow(i);
        QTableWidgetItem* cb = new QTableWidgetItem();
        if (settings.value(CONF_FILTERS_IS_DIR, CONF_DEF_FILTERS_IS_DIR).toBool()) {
            qDebug() << "checked";
            cb->setCheckState(Qt::Checked);
        } else {
            qDebug() << "unchecked";
            cb->setCheckState(Qt::Unchecked);
        }
        ui->tableWidget->setItem(i, 0, cb);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(settings.value(CONF_FILTERS_LABEL).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(settings.value(CONF_FILTERS_VALUE).toString()));
    }
    settings.endArray();

#ifdef __WIN32__
    int index = ui->priorityComboBox->findData(settings.value(CONF_PRIORITY, CONF_DEF_PRIORITY).toInt());
    ui->priorityComboBox->setCurrentIndex(index);
#endif
}

void Options::save() {
    qDebug() << "save";
    settings.setValue(CONF_MAX_OP, ui->maxOpLineEdit->text().toInt());
    settings.setValue(CONF_USE_PERIOD, ui->periodCheckBox->isChecked());
    settings.setValue(CONF_PERIOD, ui->periodLineEdit->text().toInt());
    settings.setValue(CONF_PROGRESS_REFRESH_RATE, ui->progressRefreshrateLineEdit->text().toInt());

    ((Dialog*) parent())->setBackgroundMode(ui->periodCheckBox->isChecked());

    settings.beginWriteArray(CONF_FILTERS);
    settings.remove("");
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        settings.setArrayIndex(i);
        bool isDir = (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked);
        settings.setValue(CONF_FILTERS_IS_DIR, isDir);
        settings.setValue(CONF_FILTERS_LABEL, ui->tableWidget->item(i, 1)->text());
        settings.setValue(CONF_FILTERS_VALUE, ui->tableWidget->item(i, 2)->text());
    }
    settings.endArray();


#ifdef __WIN32__
    int priority = ui->priorityComboBox->currentData().toInt();
    settings.setValue(CONF_PRIORITY, priority);
    SetPriorityClass(GetCurrentProcess(), priority);
#endif
}

void Options::reset() {
    qDebug() << "reset";
    ui->maxOpLineEdit->setText(QString::number(CONF_DEF_MAX_OP));
    ui->periodLineEdit->setText(QString::number(CONF_DEF_PERIOD));
    ui->periodCheckBox->setChecked(false);
#ifdef __WIN32__
    int index = ui->priorityComboBox->findData(CONF_DEF_PRIORITY);
    ui->priorityComboBox->setCurrentIndex(index);
#endif
}

void Options::show() {
    qDebug() << "show";
    load();
    ui->settingsTab->setCurrentIndex(0);
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

void Options::on_periodCheckBox_toggled(bool checked) {
    ui->periodLineEdit->setEnabled(checked);
}

void Options::on_addFilterButton_clicked() {
    qDebug() << "Add item";
    int rowNbr = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowNbr);
    QTableWidgetItem* cb = new QTableWidgetItem();
    cb->setCheckState(Qt::Unchecked);
    ui->tableWidget->setItem(rowNbr, 0, cb);
    ui->tableWidget->setItem(rowNbr, 1, new QTableWidgetItem);
    ui->tableWidget->setItem(rowNbr, 2, new QTableWidgetItem);
}

void Options::on_removeFilterButton_clicked() {
    QItemSelectionModel* select = ui->tableWidget->selectionModel();
    while (select->selectedIndexes().size() > 0) {
        ui->tableWidget->removeRow(select->selectedIndexes().at(0).row());
    }
}
