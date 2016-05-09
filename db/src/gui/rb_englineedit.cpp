/*****************************************************************
 * $Id: rb_englineedit.h 0 2016-04-09 19:13:00Z rutger $
 * Created: Apr 9, 2016 19:13:00 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_englineedit.h"

RB_EngLineEdit::RB_EngLineEdit(QWidget* parent) : QLineEdit(parent) {
    // nothing
}

void RB_EngLineEdit::setText(const QString& txt) {
    bool success = false;
    double value = txt.toDouble(&success);

    if (success) {
        QLineEdit::setText(QString::number(value));
    } else {
        QLineEdit::setText(txt);
    }
}
