/*****************************************************************
 * $Id: rb_uomlineedit.h 2248 2016-02-29 09:13:00Z rutger $
 * Created: Feb 29, 2016 7:50:25 AM - Rutger Botermans
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_uomlineedit.h"

#include "rb_dialogfactory.h"
#include "rb_dialogwindow.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 */
RB_UomLineEdit::RB_UomLineEdit(QWidget *parent) : QWidget(parent) {
    setupUi(this);

    mSection = -1;
    mModel = nullptr;
    QDoubleValidator* validator = new QDoubleValidator(this);
    validator->setDecimals(6);
    uomLineEdit->setValidator(validator);
}

/**
 * Destructor
 */
RB_UomLineEdit::~RB_UomLineEdit() {
    // nothing
}

/**
 * @returns LineEdit
 */
QLineEdit* RB_UomLineEdit::getLineEdit() {
    return uomLineEdit;
}

/**
 * @returns LineEdit text
 */
QString RB_UomLineEdit::text() const {
    return uomLineEdit->text();
}

/**
 * Set LineEdit text
 * @param txt text
 */
void RB_UomLineEdit::setText(const QString& txt) {
    uomLineEdit->setText(txt);
}

/**
 * Set section (column sequence number)
 * @param section
 */
void RB_UomLineEdit::setSection(int section) {
    mSection = section;
}

/**
 * Set the mapped model
 * @param model
 */
void RB_UomLineEdit::setModel(RB_MmProxy* model) {
    mModel = model;
}

void RB_UomLineEdit::setValidator(const QValidator* validator) {
    uomLineEdit->setValidator(validator);
}

void RB_UomLineEdit::setUom() {
    if (!mModel || mSection < 0) {
        RB_DEBUG->error("RB_UomLineEdit::setUom() model or section ERROR");
        return;
    }

    RB_ObjectBase* obj = mModel->getBaseObject();

    if (obj) {
        RB_ObjectMember* mem = obj->getMember(mSection);

        if (mem) {
            lblUom->setText(mem->getUnit());
        }
    }
}

void RB_UomLineEdit::setEnabled(bool enable) {
    QWidget::setEnabled(enable);
    uomLineEdit->setEnabled(enable);
}
