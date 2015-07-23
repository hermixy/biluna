/*****************************************************************
 * $Id: rb_idxlineedit.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Dec 14, 2011 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_idxlineedit.h"

#include "rb_dialogfactory.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 */
RB_IdxLineEdit::RB_IdxLineEdit(QWidget *parent) : QWidget(parent) {
    setupUi(this);

    mSection = -1;
    mModel = NULL;
    mDialogFactory = NULL;
    mDialogType = -1;
    mTargetFieldName = "";
    mFirstSourceFieldName = "";
    mSecondSourceFieldName = "";

    connect(pbSelect, SIGNAL(clicked()), this, SLOT(slotClicked()));
    // connect(pb, SIGNAL(clicked(bool)), this, SLOT(slotClicked(bool)));
    connect(pbSelect, SIGNAL(pressed()), this, SLOT(slotPressed()));
    connect(pbSelect, SIGNAL(released()), this, SLOT(slotReleased()));
    connect(pbSelect, SIGNAL(toggled(bool)), this, SLOT(slotToggled(bool)));

    connect(pbClear, SIGNAL(clicked()), this, SLOT(slotClear()));
}

/**
 * Destructor
 */
RB_IdxLineEdit::~RB_IdxLineEdit() {
    // nothing
}

/**
 * @returns LineEdit
 */
QLineEdit* RB_IdxLineEdit::getLineEdit() {
    return idxLineEdit;
}

/**
 * @returns LineEdit text
 */
QString RB_IdxLineEdit::text() const {
    return idxLineEdit->text();
}

/**
 * Set LineEdit text
 * @param txt text
 */
void RB_IdxLineEdit::setText(const QString& txt) {
    idxLineEdit->setText(txt);
}

/**
 * Set section (column sequence number)
 * @param section
 */
void RB_IdxLineEdit::setSection(int section) {
    mSection = section;
}

/**
 * Set the mapped model
 * @param model
 */
void RB_IdxLineEdit::setModel(RB_MmProxy* model) {
    mModel = model;
}

void RB_IdxLineEdit::setDefaultDialog(RB_DialogFactory* dialogFactory,
                                      int dialogType,
                                      const RB_String& targetFieldName,
                                      const RB_String& firstSourceFieldName,
                                      const RB_String& secondSourceFieldName) {
    mDialogFactory = dialogFactory;
    mDialogType = dialogType;
    mTargetFieldName = targetFieldName;
    mFirstSourceFieldName = firstSourceFieldName;
    mSecondSourceFieldName = secondSourceFieldName;
}

void RB_IdxLineEdit::setEnabled(bool enable) {
    QWidget::setEnabled(enable);
    this->pbClear->setEnabled(enable);
    this->pbSelect->setEnabled(enable);
}

/**
 * Pushbutton select clicked, emits clicked() signal
 */
void RB_IdxLineEdit::slotClicked(bool checked) {
    if (!mModel || !mDialogFactory || mDialogType < 0
            || mTargetFieldName.isEmpty() || mFirstSourceFieldName.isEmpty()) {
        RB_DEBUG->print("RB_IdxLineEdit::slotClicked() "
                        "default dialog prerequisites not set");
    } else if (!mModel->getCurrentIndex().isValid()) {
        mDialogFactory->requestWarningDialog(tr("No item selected.\n"
                                                "Please select an item first."));
    } else {
        RB_Dialog* dlg = mDialogFactory->getDialog(mDialogType);

        if (dlg->exec() != QDialog::Accepted) {
            dlg->deleteLater();
            return;
        }

        RB_ObjectBase* obj = dlg->getCurrentObject();

        if (!obj) {
            mDialogFactory->requestWarningDialog(tr("No item selected,\n"
                                                    "data unchanged."));
            dlg->deleteLater();
            return;
        }

        RB_String str = obj->getId() + obj->getValue(mFirstSourceFieldName).toString();

        if (!mSecondSourceFieldName.isEmpty()) {
            str += " - " + obj->getValue(mSecondSourceFieldName).toString();
        }

        QModelIndex idx = mModel->index(mModel->getCurrentIndex().row(),
                                        mModel->fieldIndex(mTargetFieldName));
        mModel->setData(idx, str);
        dlg->deleteLater();
    }

    emit clicked(checked);
}

/**
 * Pushbutton select pressed, emits pressed() signal
 */
void RB_IdxLineEdit::slotPressed() {
    emit pressed();
}

/**
 * Pushbutton select released, emits released() signal
 */
void RB_IdxLineEdit::slotReleased() {
    emit released();
}

/**
 * Pushbutton select toggled, emits toggled() signal
 */
void RB_IdxLineEdit::slotToggled(bool checked) {
    emit toggled(checked);
}

/**
 * Pushbutton clear clicked, clears the data in the model
 * and than emits clear() signal
 */
void RB_IdxLineEdit::slotClear() {
    if (!mModel || mSection < 0) return;

    QModelIndex idx = mModel->getProxyIndex();
    if (!idx.isValid()) return;

    idx = mModel->index(idx.row(), mSection, idx.parent());
    mModel->setData(idx, "0", Qt::EditRole);

    emit clear();
}
