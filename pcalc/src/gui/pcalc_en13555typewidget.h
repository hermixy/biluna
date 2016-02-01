/*****************************************************************
 * $Id: pcalc_selecten13555manufwidget.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Feb 01, 2016 21:25:40 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN13555TYPEWIDGET_H
#define PCALC_EN13555TYPEWIDGET_H

#include <QtWidgets>
#include "pcalc_modelfactory.h"
#include "db_tablewidget.h"
#include "db_modelfactory.h"


/**
 * @brief Widget to select and edit a type of EN13555 gasket.
 * Currently used as a dialog
 */
class PCALC_EN13555TypeWidget : public DB_TableWidget {

    Q_OBJECT

public:
    PCALC_EN13555TypeWidget(QWidget* parent);
    virtual ~PCALC_EN13555TypeWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectivePCALC;
    }

    void init();
    // already in DB_TableWidget
    // bool fileSave(bool withSelect);
    // void fileRevert();

    void on_pbAdd_clicked();
};

#endif // PCALC_EN13555TYPEWIDGET_H
