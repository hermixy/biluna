/*****************************************************************
 * $Id: rb_plaintextedit.cpp 1964 2013-08-06 17:35:10Z rutger $
 * Created: 2012-12-12 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_plaintextedit.h"

#include <QPrinter>

/**
 * Constructor
 */
RB_PlainTextEdit::RB_PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent) {
    // nothing
}

/**
 * Print this textEdit widget
 * @param printer
 */
void RB_PlainTextEdit::print(QPrinter* printer) {
    document()->setUseDesignMetrics(true);
    document()->print(printer);
}
