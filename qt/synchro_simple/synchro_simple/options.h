#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

#include "settings.h"

namespace Ui {
class Options;
}

class Options : public QDialog {
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

public slots:
    void show();
    void accept();
    void reject();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_periodCheckBox_toggled(bool checked);

private:
    Ui::Options *ui;
    QSettings settings;

    void save();
    void load();
    void reset();
};

#endif // OPTIONS_H
