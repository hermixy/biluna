/*****************************************************************
 * $Id: db_spreadsheetwidget.cpp 2248 2016-08-09 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_spreadsheetwidget.h"
#include "rb_spreadsheettable.h"
#include "ui_db_spreadsheetwidget.h"

DB_SpreadSheetWidget::DB_SpreadSheetWidget(QWidget *parent) :
        RB_Widget(parent), ui(new Ui::DB_SpreadSheetWidget) {
    ui->setupUi(this);
}

DB_SpreadSheetWidget::~DB_SpreadSheetWidget() {
    delete ui;
}

/**
 * Get name for MDI window title
 * @return file name (or type of window in case of unique window),
 * the file name is empty at opening a new window and replaced
 * by the file name when the text file is saved. When opening an
 * existing file the file name will be returned.
 */
QString DB_SpreadSheetWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }

    return QFileInfo(mFullFileName).fileName();
}

void DB_SpreadSheetWidget::init() {
    ui->spreadSheetTable->clear();
    ui->spreadSheetTable->setAutoRecalculate(true);
}
