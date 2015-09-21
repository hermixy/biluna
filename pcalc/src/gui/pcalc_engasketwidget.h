/*****************************************************************
 * $Id: pcalc_actionen1591flange.h 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ENGASKETWIDGET_H
#define PCALC_ENGASKETWIDGET_H

#include "rb_databasetablewidget.h"

class PCALC_ENGasketWidget : public RB_DatabaseTableWidget {

public:
    PCALC_ENGasketWidget(QWidget *parent = 0);

    QString getName() const { return QString(); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }
};

#endif // PCALC_ENGASKETWIDGET_H
