/*****************************************************************
 * $Id: db_aboutdialog.cpp 1309 2010-10-18 21:58:58Z rutger $
 * Created: Nov 26 2009 4:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_aboutdialog.h"
#include <QLabel>

/**
 * Constructor
 */
DB_AboutDialog::DB_AboutDialog(QWidget* parent) : RB_Dialog(parent) {
    setupUi(this);
    readSettings();
}

/**
 * Destructor
 */
DB_AboutDialog::~DB_AboutDialog() {
    // nothing
}

/**
 * Sets the text of the about dialog
 */
void DB_AboutDialog::setText(const QString& s) {
    lblText->setText(s);
}

/**
 * Sets the icon of the about dialog
 */
void DB_AboutDialog::setIcon(const QPixmap& i) {
    lblIcon->setPixmap(i);
}
