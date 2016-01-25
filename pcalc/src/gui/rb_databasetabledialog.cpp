#include "rb_databasetabledialog.h"
#include "ui_rb_databasetabledialog.h"

RB_DatabaseTableDialog::RB_DatabaseTableDialog(QWidget *parent)
    : RB_Dialog(parent), ui(new Ui::RB_DatabaseTableDialog) {

    ui->setupUi(this);
}

RB_DatabaseTableDialog::~RB_DatabaseTableDialog() {
    delete ui;
}
