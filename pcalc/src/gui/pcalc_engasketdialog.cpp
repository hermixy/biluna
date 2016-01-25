/*****************************************************************
 * $Id: pcalc_actionen1591flange.h 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_engasketdialog.h"
#include "rb_databasetablewidget.h"

PCALC_ENGasketDialog::PCALC_ENGasketDialog(QWidget* parent)
    : RB_Dialog(parent) {

    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->addWidget(new RB_DatabaseTableWidget(this));
    setLayout(layout);
}

