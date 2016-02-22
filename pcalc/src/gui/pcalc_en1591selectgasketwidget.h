/*****************************************************************
 * $Id: pcalc_en1591selectgasketwidget.h 2234 2016-02-22 19:23:20Z rutger $
 * Created: Feb 22, 2016 4:34:42 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN1591SELECTGASKETWIDGET_H
#define PCALC_EN1591SELECTGASKETWIDGET_H

#include "rb_widget.h"

namespace Ui {
class PCALC_EN1591SelectGasketWidget;
}

/**
 * Dialog window widget to select gasket for EN1591 calculation
 */
class PCALC_EN1591SelectGasketWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit PCALC_EN1591SelectGasketWidget(QWidget* parent = 0);
    virtual ~PCALC_EN1591SelectGasketWidget();

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("EN1591 Select Gasket"); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }

    RB_ObjectBase* getCurrentObject();
    void init();

    // Catch signals from RB_DialogWindow
    bool fileSave(bool /*withSelect*/) { return false; }
    void fileRevert() { }

private:
    Ui::PCALC_EN1591SelectGasketWidget* ui;

    RB_MmProxy* mManufModel;
    RB_MmProxy* mGasketModel;
};

#endif // PCALC_EN1591SELECTGASKETWIDGET_H
