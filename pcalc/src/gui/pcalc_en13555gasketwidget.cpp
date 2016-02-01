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
#include "pcalc_modelfactory.h"
#include "rb_dialogwindow.h"

PCALC_EN13555GasketWidget::PCALC_EN13555GasketWidget(QWidget* parent)
    : RB_DatabaseTableWidget(parent) {
    // TODO
}

PCALC_EN13555GasketWidget::~PCALC_EN13555GasketWidget() {
    RB_DEBUG->print("PCALC_EN13555GasketWidget::~PCALC_EN13555GasketWidget() OK");
}

void PCALC_EN13555GasketWidget::init() {

}

bool PCALC_EN13555GasketWidget::fileSave(bool withSelect) {
    // TODO:
    return true;
}

void PCALC_EN13555GasketWidget::fileRevert() {
    // TODO:
    return;
}

QString PCALC_EN13555GasketWidget::getHelpSubject() const {
    return objectName();
}

void PCALC_EN13555GasketWidget::on_pbSelectManuf_clicked() {
    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetEN13555Manuf);


    if (dlg->exec() == QDialog::Rejected) {
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();
    setCodeManufacturer(obj);
    dlg->deleteLater();
}

void PCALC_EN13555GasketWidget::on_pbClearManuf_clicked() {
    setCodeManufacturer(nullptr);
}

void PCALC_EN13555GasketWidget::on_pbSelectType_clicked() {
    // Is valid parent ID set for this dialog
    if (mCodeManufId.size() < 38) {
        PCALC_DIALOGFACTORY->requestInformationDialog(
                    tr("Not a valid manufacturer selected\n"));
        return;
    }

    // Set parent ID for dialog
    PCALC_MODELFACTORY->setStoredId(mCodeManufId);

    RB_DialogWindow* dlg = PCALC_DIALOGFACTORY->getDialogWindow(
                PCALC_DialogFactory::WidgetEN13555Type);


    if (dlg->exec() == QDialog::Rejected) {
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();
    setType(obj);
    dlg->deleteLater();
}

void PCALC_EN13555GasketWidget::on_pbClearType_clicked() {
    setType(nullptr);
}

