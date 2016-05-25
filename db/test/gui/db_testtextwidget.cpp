/*****************************************************************
 * $Id: db_testtextwidget.cpp 1050 2009-12-03 22:38:27Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testtextwidget.h"

DB_TestTextWidget::DB_TestTextWidget(QWidget* parent) : RB_Widget(parent){
    setupUi(this);

    connect(textEdit, SIGNAL(textChanged()),
            this, SLOT(slotTextIsChanged()));
}

void DB_TestTextWidget::init() {
    readSettings();
}

/**
 * Slot: clear text widget
 */
void DB_TestTextWidget::slotClearText() {
    textEdit->clear();
}

/**
 * Slot: text has changed, to transmit signal to MDI window
 */
void DB_TestTextWidget::slotTextIsChanged() {
    slotDataIsChanged(QModelIndex(), QModelIndex());
}

void DB_TestTextWidget::appendText(const RB_String& str) {
    textEdit->append(str);
}

void DB_TestTextWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
