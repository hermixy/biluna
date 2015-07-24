/*****************************************************************
 * $Id: crm_groupcontactwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 5, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_groupcontactwidget.h"

#include <QStringListModel>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "crm_sqlcommonfunctions.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
CRM_GroupContactWidget::CRM_GroupContactWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mGroupModel = NULL;
    mGroupMapper = NULL;
    mContactModel = NULL;
    mContactMapper = NULL;

}

/**
 * Destructor
 */
CRM_GroupContactWidget::~CRM_GroupContactWidget() {
    if (mContactModel) {
        delete mContactModel;
    }
    if (mGroupModel) {
        delete mGroupModel;
    }
    RB_DEBUG->print("CRM_GroupContactWidget::~CRM_GroupContactWidget() OK");
}

/**
 * Initialize widget and models
 */
void CRM_GroupContactWidget::init() {
    //
    // 1. Set model for customer mapper
    //
    mGroupModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelGroup); // shared
    mGroupModel->setRoot(CRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mGroupMapper = mGroupModel->getMapper();

    mGroupMapper->addMapping(leName, mGroupModel->fieldIndex("mname"));
    mGroupMapper->addMapping(leDescription, mGroupModel->fieldIndex("description"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mGroupModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvGroup, mGroupModel);

    // Child model and views
    mContactModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelGroupContact); // shared
    mContactMapper = mContactModel->getMapper();
    mContactMapper->addMapping(ileContact, mContactModel->fieldIndex("contact_idx"));
    mContactMapper->addMapping(leNote, mContactModel->fieldIndex("note"));
    setFormatTableView(tvContact, mContactModel);
    readSettings();

    int count = mGroupModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mGroupModel->fieldIndex("mname")
                && i != mGroupModel->fieldIndex("description")) {
            tvGroup->hideColumn(i);
        } else {
            tvGroup->showColumn(i);

            if (tvGroup->columnWidth(i) < 5) {
                tvGroup->setColumnWidth(i, 100);
            }
        }
    }

    count = mContactModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1) {
            tvContact->hideColumn(i);
        } else {
            tvContact->showColumn(i);

            if (tvContact->columnWidth(i) < 5) {
                tvContact->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool CRM_GroupContactWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mGroupModel) {
        if (mGroupModel->submitAllAndSelect()
                && mContactModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mGroupModel) {
        if (mGroupModel->submitAll() && mContactModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add clicked
 */
void CRM_GroupContactWidget::on_pbAddGroup_clicked() {
    if (!mGroupModel) return;

    if (isWindowModified()) {
        fileSave(true);
    }

    mGroupModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mGroupModel->rowCount();
    mGroupModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mGroupModel->index(row, mGroupModel->fieldIndex("mname"));
    mGroupModel->setData(idx, "<NEW>", Qt::EditRole);
    // end NOTE

    tvGroup->setCurrentIndex(mGroupModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvGroup->scrollTo(tvGroup->currentIndex());
    leName->setFocus();
    leName->selectAll();
}

/**
 * Button delete clicked
 */
void CRM_GroupContactWidget::on_pbDeleteGroup_clicked() {
    int ret = CRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        CRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvGroup->currentIndex();
    if (!index.isValid()) return;

    mGroupModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void CRM_GroupContactWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_GroupContactWidget::on_pbFilterOnGroup_clicked() {
    RB_String str = leFilterGroup->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`crm_group`.`mname` LIKE '";
    filter += str + "'";
    mGroupModel->setWhere(filter);
}

void CRM_GroupContactWidget::on_pbFilterOffGroup_clicked() {
    mGroupModel->setWhere("");
    leFilterGroup->setText("");
}

/**
 * Pushbutton Add Contact clicked
 */
void CRM_GroupContactWidget::on_pbAddContact_clicked() {
    if (!mContactModel) return;
    mContactModel->undoFilterSort();

    // always insert at the begin
    int row = 0;
    mContactModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mContactModel->index(row, mContactModel->fieldIndex("contact_idx"));
    mContactModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    tvContact->setCurrentIndex(mContactModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvContact->scrollTo(tvContact->currentIndex());
    ileContact->setFocus();
}

/**
 * Pushbutton Delete Contact clicked
 */
void CRM_GroupContactWidget::on_pbDeleteContact_clicked() {
    int ret = CRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        CRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvContact->currentIndex();
    if (!index.isValid()) return;

    mContactModel->removeRows(index.row(), 1, QModelIndex());
    mContactModel->submitAllAndSelect();
    // setWindowModified(false); parent could have changed
}

/**
 * Pushbutton Filter On Contact clicked
 */
void CRM_GroupContactWidget::on_pbFilterOnContact_clicked() {
    RB_String str = leFilterContact->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }

    CRM_SqlCommonFunctions util;
    RB_String filter = "";
    util.substrIdxContent("crm_groupcontact.contact_idx", filter);
    filter += " LIKE '";
    filter += str + "'";
    mContactModel->setWhere(filter);
}

/**
 * Pushbutton Filter Off Contact clicked
 */
void CRM_GroupContactWidget::on_pbFilterOffContact_clicked() {
    mContactModel->setWhere("");
    leFilterContact->setText("");
}

/**
 * Button select group clicked
 */
void CRM_GroupContactWidget::on_ileContact_clicked() {
    if (!tvGroup->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No group selected.\n"
                                                   "Please select a group first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectContact);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();

        if (obj) {
            // The parent field has been replaced in the dialog to ACC_Customer.customercode
            RB_String custCode = obj->getValue("parent").toString();

            int row = tvContact->currentIndex().row();
            QModelIndex index = mContactModel->index(row, mContactModel->fieldIndex("contact_idx"));
            mContactModel->setData(index, obj->getId()
                                  + obj->getValue("firstname").toString() + " "
                                  + obj->getValue("lastname").toString()
                                   + " (" + custCode + ")", Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No Contact selected.\n"
                                                   "contact is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_GroupContactWidget::getHelpSubject() const {
    return objectName();
}

bool CRM_GroupContactWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_GroupContactWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
