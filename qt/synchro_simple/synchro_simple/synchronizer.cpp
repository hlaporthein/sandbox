#include "synchronizer.h"
#include "dialog.h"
#include "ui_dialog.h"
#include <windows.h>

Synchronizer::Synchronizer(Dialog* d) :
    dialog(d)
{
}

Synchronizer::~Synchronizer() {
    // free resources
}

void Synchronizer::process() {
    my_print("Hello World!");
    my_print(dialog->ui->srcLineEdit->text().toLocal8Bit().data());
    Sleep(5000);
    emit finished();
}

//void Synchronizer::finished() {}

//void Synchronizer::error(QString err) {}
