/*****************************************************************
 * $Id: pcalc_cadview.h 2241 2016-08-28 10:22:19Z rutger $
 * Created: Aug 28, 2016 - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN1591CADVIEW_H
#define PCALC_EN1591CADVIEW_H

#include "pcalc_cadview.h"
#include "rb_objectbase.h"

/**
 * @brief The PCALC_EN1591CadView wrapper class based on PCALC_CadView
 */
class PCALC_EN1591CadView : public PCALC_CadView {

    Q_OBJECT

public:
    explicit PCALC_EN1591CadView(QWidget* parent = 0);

    void drawAll(RB_ObjectBase* flangeObj, RB_ObjectBase* gasketObj,
                 RB_ObjectBase* boltNutWasherObj, RB_ObjectBase* shellObj);
    void drawFlanges(RB_ObjectBase* flangeObj);
    void drawGasket(RB_ObjectBase* gasketObj);
    void drawBoltNutWasher(RB_ObjectBase* boltNutWasherObj);
    void drawShells(RB_ObjectBase* shellObj);

    void drawTestLine();

signals:

public slots:

private:
    void initView();

    RS_Graphic* mGraphic;
};

#endif // PCALC_EN1591CADVIEW_H
