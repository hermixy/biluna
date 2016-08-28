/*****************************************************************
 * $Id: PCALC_EN1591CadView.cpp 2241 2016-08-28 10:22:19Z rutger $
 * Created: Aug 28, 2016 - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591cadview.h"


PCALC_EN1591CadView::PCALC_EN1591CadView(QWidget* parent)
            : PCALC_CadView(parent) {
    initView();
}

/**
 * @brief Initialize view with remark: "Click tabs for drawing"
 */
void PCALC_EN1591CadView::initView() {
    mGraphic = getGraphic();
    mGraphic->addEntity(new RS_Text(mGraphic,
                                    RS_TextData(RS_Vector(10,10), 5, 100,
                                                RS2::VAlignBottom,
                                                RS2::HAlignLeft,
                                                RS2::LeftToRight,
                                                RS2::AtLeast,
                                                1.0,
                                                "Click tabs for drawing",
                                                "standard",
                                                0.0,
                                                RS2::Update)));
}

void PCALC_EN1591CadView::drawAll(RB_ObjectBase* flangeObj,
                                  RB_ObjectBase* gasketObj,
                                  RB_ObjectBase* boltNutWasherObj,
                                  RB_ObjectBase* shellObj) {
    mGraphic->clear();
    addLine(0, 0, 100, 50);
    zoomAuto(true, true);
}

void PCALC_EN1591CadView::drawFlanges(RB_ObjectBase* flangeObj) {
    mGraphic->clear();
    addLine(0, 0, 100, 60);
    zoomAuto(true, true);
}

void PCALC_EN1591CadView::drawGasket(RB_ObjectBase* gasketObj) {
    mGraphic->clear();
    addLine(0, 0, 100, 70);
    zoomAuto(true, true);
}

void PCALC_EN1591CadView::drawBoltNutWasher(RB_ObjectBase* boltNutWasherObj) {
    mGraphic->clear();
    addLine(0, 0, 100, 80);
    zoomAuto(true, true);
}

void PCALC_EN1591CadView::drawShells(RB_ObjectBase* shellObj) {
    mGraphic->clear();
    addLine(0, 0, 100, 90);
    zoomAuto(true, true);
}

void PCALC_EN1591CadView::drawTestLine() {
    mGraphic->clear();
    addLine(0, 0, 100, 50);
    zoomAuto(true, true);
}


