#include "dialog.h"
#include "ui_dialog.h"
#include <QApplication>
#include <QScrollBar>
#include <QDebug>
extern "C" {
    #include "../../../copy_struct_test/synchro.h"
}

Ui::Dialog* g_uip;

void my_print(const char* buf) {
    qDebug() << "buf=" << buf;
    g_uip->traceTextEdit->moveCursor(QTextCursor::End);
    g_uip->traceTextEdit->insertPlainText(QString(buf));
    QCoreApplication::processEvents();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    set_print(my_print);
    return a.exec();
}
