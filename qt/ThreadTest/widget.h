#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWaitCondition>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Ui::Widget* getUi() { return ui; }

private slots:
    void on_pushButton_clicked();
    void errorString(QString str);
    void print(const char* buf);
    void enableSyncButton();

private:
    Ui::Widget *ui;
    QWaitCondition canContinue;
};

#endif // WIDGET_H
