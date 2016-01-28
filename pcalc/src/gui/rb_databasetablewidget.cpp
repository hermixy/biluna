#include "rb_databasetablewidget.h"
#include "ui_rb_databasetablewidget.h"

RB_DatabaseTableWidget::RB_DatabaseTableWidget(QWidget *parent)
    : RB_Widget(parent), ui(new Ui::RB_DatabaseTableWidget) {

    ui->setupUi(this);
    ui->tbbData->initSlimTable(true, false, true);
}

RB_DatabaseTableWidget::~RB_DatabaseTableWidget() {
    delete ui;
}

void RB_DatabaseTableWidget::init() {
//    ui->tbbData->initSlimTable(true, false, true);
}
