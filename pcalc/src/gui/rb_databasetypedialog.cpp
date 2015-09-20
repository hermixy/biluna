#include "rb_databasetypedialog.h"
#include "ui_rb_databasetypedialog.h"

RB_DatabaseTypeDialog::RB_DatabaseTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RB_DatabaseTypeDialog)
{
    ui->setupUi(this);
}

RB_DatabaseTypeDialog::~RB_DatabaseTypeDialog()
{
    delete ui;
}
