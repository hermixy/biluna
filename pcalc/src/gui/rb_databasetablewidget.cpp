#include "rb_databasetablewidget.h"
#include "ui_rb_databasetablewidget.h"

RB_DatabaseTableWidget::RB_DatabaseTableWidget(QWidget *parent)
    : RB_Widget(parent), ui(new Ui::RB_DatabaseTableWidget) {

    ui->setupUi(this);
}

RB_DatabaseTableWidget::~RB_DatabaseTableWidget() {
    delete ui;
}
