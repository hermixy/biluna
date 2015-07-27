/*****************************************************************
 * $Id: gv_onetimedialog.cpp 1309 2010-10-18 21:58:58Z rutger $
 * Created: Feb 4, 2009 9:53:26 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_onetimedialog.h"

/**
 * Constructor
 */
GV_OneTimeDialog::GV_OneTimeDialog(QWidget* parent)
        : RB_Dialog(parent) {
	
    setupUi(this);
    readSettings();
}

/**
 * Destructor
 */
GV_OneTimeDialog::~GV_OneTimeDialog() {
}


void GV_OneTimeDialog::setText(const RB_String& text) {
    //lblText->resize(lblText->width(), text.count(RB_String("\n"))*12);
    lblText->setText(text);
}


bool GV_OneTimeDialog::getDontShowAgain() {
    return cbDontShowAgain->isChecked();
}

