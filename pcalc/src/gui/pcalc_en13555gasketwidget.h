/*****************************************************************
 * $Id: pcalc_en13555gasketwidget.h 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555GASKETWIDGET_H
#define PCALC_EN13555GASKETWIDGET_H

#include "rb_databasetablewidget.h"

class PCALC_EN13555GasketWidget : public RB_DatabaseTableWidget {

    Q_OBJECT

public:
    PCALC_EN13555GasketWidget(QWidget *parent = 0);

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("EN13555 Gasket Properties"); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }

    void init();
    bool fileSave(bool withSelect);

    QString getHelpSubject() const;

};

#endif // PCALC_EN13555GASKETWIDGET_H
