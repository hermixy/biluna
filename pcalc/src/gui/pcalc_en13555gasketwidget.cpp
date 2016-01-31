/*****************************************************************
 * $Id: pcalc_en13555gasketwidget.cpp 2234 2015-04-30 19:23:20Z rutger $
 * Created: Sept 15, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en13555gasketwidget.h"

#include "pcalc_dialogfactory.h"
#include "rb_dialogwindow.h"

PCALC_EN13555GasketWidget::PCALC_EN13555GasketWidget(QWidget* parent)
    : RB_DatabaseTableWidget(parent) {
    // TODO
}

void PCALC_EN13555GasketWidget::init() {
    connect(this, SIGNAL(pbSelectManufClicked()),
            this, SLOT(slotPbSelectManuf_clicked()));
    connect(this, SIGNAL(pbClearManufClicked()),
            this, SLOT(slotPbClearManuf_clicked()));
    connect(this, SIGNAL(pbSelectTypeClicked()),
            this, SLOT(slotPbSelectType_clicked()));
    connect(this, SIGNAL(pbClearTypeClicked()),
            this, SLOT(slotPbClearType_clicked()));
}

bool PCALC_EN13555GasketWidget::fileSave(bool withSelect) {
    return true;
}

QString PCALC_EN13555GasketWidget::getHelpSubject() const {
    return objectName();
}

void PCALC_EN13555GasketWidget::slotPbSelectManuf_clicked() {
    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetEN13555Manuf);


    if (dlg->exec() == QDialog::Rejected) {
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();
    setCodeManufacturer(obj);
    dlg->deleteLater();
}

void PCALC_EN13555GasketWidget::slotPbClearManuf_clicked() {
    setCodeManufacturer(nullptr);
}

void PCALC_EN13555GasketWidget::slotPbSelectType_clicked() {

}

void PCALC_EN13555GasketWidget::slotPbClearType_clicked() {

}

