#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_srcBrowseButton_clicked();

    void on_dstBrowseButton_clicked();

    void on_syncButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
