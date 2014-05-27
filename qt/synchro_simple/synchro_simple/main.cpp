#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QScrollBar>
extern "C" {
    #include "../../../copy_struct_test/synchro.h"
}

Ui::Dialog* g_uip;

void my_print(const char* buf) {
    g_uip->traceTextEdit->insertPlainText(buf);
    QScrollBar* sb = g_uip->traceTextEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    set_print(my_print);
    return a.exec();
}
