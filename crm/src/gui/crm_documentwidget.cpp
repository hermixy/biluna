/*****************************************************************
 * $Id: crm_documentwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 27, 2014 3:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_documentwidget.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "crm_selectcontactdialog.h"
#include "crm_selectcustomerdialog.h"
#include "crm_sqlcommonfunctions.h"
#include "crm_template.h"
#include "acc_contact.h"
#include "acc_customer.h"
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_printtextheaderfooter.h"
#include "rb_sqlrelationaldelegate.h"



/**
 * Constructor
 */
CRM_DocumentWidget::CRM_DocumentWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
CRM_DocumentWidget::~CRM_DocumentWidget() {
    delete mModel; // do not forget to delete otherwise application crashes
    RB_DEBUG->print("CRM_DocumentWidget::~CRM_DocumentWidget() OK");
}

/**
 * Get proposed saveas filename
 * @returns filename
 */
RB_String CRM_DocumentWidget::getSaveAsFileName() {
    if (!mModel->getCurrentIndex().isValid()) {
        return QString();
    }

    RB_String str = mModel->getCurrentValue("refno").toString();
    return str;
}

/**
 * Initialize widget and models
 */
void CRM_DocumentWidget::init() {

    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    //
    // 1. Set model for customer mapper
    //
    mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelDocument, false);
    mModel->setRoot(CRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    mMapper->addMapping(sbRefNo, mModel->fieldIndex("refno"));
    mMapper->addMapping(leSubject, mModel->fieldIndex("subject"));
    mMapper->addMapping(ileCustomer, mModel->fieldIndex("customer_idx"));
    mMapper->addMapping(ileContact, mModel->fieldIndex("contact_idx"));
    mMapper->addMapping(ileTemplate, mModel->fieldIndex("template_idx"));
    mMapper->addMapping(teContent, mModel->fieldIndex("content"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvDocument, mModel);

    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mModel->fieldIndex("refno")
                && i != mModel->fieldIndex("subject")
                && i != mModel->fieldIndex("customer_idx")
                && i != mModel->fieldIndex("contact_idx")
                && i != mModel->fieldIndex("template_idx")) {
            tvDocument->hideColumn(i);
        } else {
            tvDocument->showColumn(i);

            if (tvDocument->columnWidth(i) < 5) {
                tvDocument->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data to database
 */
bool CRM_DocumentWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {

            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

void CRM_DocumentWidget::filePdf(QPrinter* pr) {
    if (isWindowModified()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr);
}

void CRM_DocumentWidget::filePrint(QPrinter* pr) {
    if (isWindowModified()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr);
}

void CRM_DocumentWidget::filePrintPreview(QPrinter* pr) {
    if (isWindowModified()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr, true);
}

/**
 * Edit Cut.
 */
void CRM_DocumentWidget::editCut() {
    teContent->cut();
}

/**
 * Edit Copy.
 */
void CRM_DocumentWidget::editCopy() {
    teContent->copy();
}

/**
 * Edit Paste.
 */
void CRM_DocumentWidget::editPaste() {
    teContent->paste();
}

/**
 * Edit Undo.
 */
void CRM_DocumentWidget::editUndo() {
    teContent->undo();
}

/**
 * Edit Redo.
 */
void CRM_DocumentWidget::editRedo() {
    teContent->redo();
}

/**
 * Select All.
 */
void CRM_DocumentWidget::editSelectAll() {
    teContent->selectAll();
}

/**
 * Format Bold
 */
void CRM_DocumentWidget::formatBold() {
    teContent->slotTextBold();
}

/**
 * Format Italic
 */
void CRM_DocumentWidget::formatItalic() {
    teContent->slotTextItalic();
}

/**
 * Format Underline
 */
void CRM_DocumentWidget::formatUnderline() {
    teContent->slotTextUnderline();
}

/**
 * Format Strikethrough
 */
void CRM_DocumentWidget::formatStrikethrough() {
    teContent->slotTextStrikeOut();
}

/**
 * Format Align Left
 */
void CRM_DocumentWidget::formatAlignLeft() {
    teContent->slotTextAlign(RB_TextEdit::AlignLeft);
}

/**
 * Format Align Center
 */
void CRM_DocumentWidget::formatAlignCenter() {
    teContent->slotTextAlign(RB_TextEdit::AlignCenter);
}

/**
 * Format Align Right
 */
void CRM_DocumentWidget::formatAlignRight() {
    teContent->slotTextAlign(RB_TextEdit::AlignRight);
}

/**
 * Format Align Justify
 */
void CRM_DocumentWidget::formatAlignJustify() {
    teContent->slotTextAlign(RB_TextEdit::AlignJustify);
}

/**
 * Format Increase Indent
 */
void CRM_DocumentWidget::formatIncreaseIndent() {
    teContent->slotTextIndent(1);
}

/**
 * Format Decrease Indent
 */
void CRM_DocumentWidget::formatDecreaseIndent() {
    teContent->slotTextIndent(-1);
}

/**
 * Format numbered list
 */
void CRM_DocumentWidget::formatNumberedList() {
    teContent->slotTextStyle(RB_TextEdit::ListDecimal);
}

/**
 * Format bulleted list
 */
void CRM_DocumentWidget::formatBulletedList() {
    teContent->slotTextStyle(RB_TextEdit::ListDisc);
}

/**
 * Format font family
 */
void CRM_DocumentWidget::formatFontName() {
    teContent->slotTextFamily();
}

/**
 * Format font size
 */
void CRM_DocumentWidget::formatFontSize() {
    teContent->slotTextSize();
}

/**
 * Format text color
 */
void CRM_DocumentWidget::formatTextColor() {
    teContent->slotTextColor();
}

/**
 * Format background color
 */
void CRM_DocumentWidget::formatBackgroundColor() {
    teContent->slotBackgroundColor();
}

/**
 * Remove formatting and style
 */
void CRM_DocumentWidget::formatRemove() {
    //    webView->triggerPageAction(QWebPage::RemoveFormat);
}

void CRM_DocumentWidget::print(QPrinter* pr, bool isPreview) {
    if (!tvDocument->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    RB_PrintTextHeaderFooter* utilPrint = new RB_PrintTextHeaderFooter();

    RB_ObjectBase* docObj = mModel->getCurrentObject();
    preparePrintText(utilPrint, docObj);
    utilPrint->print(pr);

    if (!isPreview) {
        addLogActivity(docObj);
    }

    delete utilPrint;
    QApplication::restoreOverrideCursor();

}

void CRM_DocumentWidget::addLogActivity(RB_ObjectBase* docObj) {
    RB_MmProxy* activityModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelActivity);
    if (!activityModel) return;

    RB_String dt = QDateTime::currentDateTime().date().toString(Qt::ISODate);
    int row = 0;
    activityModel->insertRows(row, 1);

    QModelIndex idx;
    idx = activityModel->index(row, activityModel->fieldIndex("activitytype_id"));
    activityModel->setData(idx, 3); // log

    int modelRow = mModel->getCurrentIndex().row();
    QModelIndex index = mModel->index(modelRow, mModel->fieldIndex("customer_idx"));
    QString customerCodeName = mModel->data(index).toString();
    QStringList customerList = customerCodeName.split(" ");
    idx = activityModel->index(row, activityModel->fieldIndex("activitycode"));
    activityModel->setData(idx, customerList.at(0) + "LOG" + dt);

    idx = activityModel->index(row, activityModel->fieldIndex("subject"));
    activityModel->setData(idx, tr("Mail:") + " " + docObj->getValue("refno").toString());
    dt = QDateTime::currentDateTime().toString(Qt::ISODate);
    idx = activityModel->index(row, activityModel->fieldIndex("mstart"));
    activityModel->setData(idx, dt);
    idx = activityModel->index(row, activityModel->fieldIndex("mend"));
    activityModel->setData(idx, dt);
    idx = activityModel->index(row, activityModel->fieldIndex("isreminder"));
    activityModel->setData(idx, 0);
    idx = activityModel->index(row, activityModel->fieldIndex("remindertime"));
    activityModel->setData(idx, 0);
    idx = activityModel->index(row, activityModel->fieldIndex("priority_id"));
    activityModel->setData(idx, 0);
    idx = activityModel->index(row, activityModel->fieldIndex("description"));
    activityModel->setData(idx, tr("Content:") + " " + docObj->getValue("content").toString());
    idx = activityModel->index(row, activityModel->fieldIndex("status_id"));
    activityModel->setData(idx, 0);
    idx = activityModel->index(row, activityModel->fieldIndex("location"));
    activityModel->setData(idx, tr("None"));
    idx = activityModel->index(row, activityModel->fieldIndex("mvalue"));
    activityModel->setData(idx, 0);

    index = mModel->index(modelRow, mModel->fieldIndex("contact_idx"));
    QString contactId = mModel->data(index, RB2::RoleOrigData).toString();
    idx = activityModel->index(row, activityModel->fieldIndex("acc_contact_id"));
    activityModel->setData(idx, contactId);

    index = mModel->index(modelRow, mModel->fieldIndex("customer_idx"));
    QString customerId = mModel->data(index, RB2::RoleOrigData).toString();
    idx = activityModel->index(row, activityModel->fieldIndex("acc_customer_id"));
    activityModel->setData(idx, customerId);

}

/**
 * Button add clicked
 */
void CRM_DocumentWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // Set customer if already selected
    if (CRM_DIALOGFACTORY->isWidgetActive(CRM_DialogFactory::WidgetCustomer)) {
        RB_Widget* wgt = CRM_DIALOGFACTORY->getWidget(CRM_DialogFactory::WidgetCustomer, NULL);
        QModelIndex index = wgt->getCurrentModelIndex(CRM_ModelFactory::ModelCustomer);

        if (index.isValid()) {
            RB_MmProxy* model = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelCustomer);
            index = model->index(index.row(), model->fieldIndex("id"));
            QString uuid = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("customerno"));
            QString code = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("mname"));
            QString name = index.data(Qt::EditRole).toString();

            index = mModel->index(row, mModel->fieldIndex("customer_idx"));
            mModel->setData(index, uuid + code + " " + name, Qt::EditRole);
        }
    }

    // Set contact if already selected
    if (CRM_DIALOGFACTORY->isWidgetActive(CRM_DialogFactory::WidgetContact)) {
        RB_Widget* wgt = CRM_DIALOGFACTORY->getWidget(CRM_DialogFactory::WidgetContact, NULL);
        QModelIndex index = wgt->getCurrentModelIndex(CRM_ModelFactory::ModelContact);

        if (index.isValid()) {
            RB_MmProxy* model = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelContact);
            index = model->index(index.row(), model->fieldIndex("id"));
            QString uuid = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("firstname"));
            QString firstName = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("lastname"));
            QString lastName = index.data(Qt::EditRole).toString();

            index = mModel->index(row, mModel->fieldIndex("contact_idx"));
            mModel->setData(index, uuid + firstName + " " + lastName, Qt::EditRole);
        }
    }

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index;
    index = mModel->index(row, mModel->fieldIndex("subject"));
    mModel->setData(index, "<NEW>", Qt::EditRole);
    // end NOTE

    // number, set (subject) and (do not select contact if not part of customer if customer selected)
    CRM_SqlCommonFunctions oper;
    int number = 0;
    number = oper.getNextSystemSeqNumber(mModel->getModelType(), "Document Number");
    index = mModel->index(row, mModel->fieldIndex("refno"));
    mModel->setData(index, number, Qt::EditRole);


    tvDocument->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvDocument->scrollTo(tvDocument->currentIndex());
    leSubject->setFocus();
    leSubject->selectAll();
}

/**
 * Button delete clicked
 */
void CRM_DocumentWidget::on_pbDelete_clicked() {
    int ret = CRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete document ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this document?"));
    if (ret == QMessageBox::No) {
        CRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvDocument->currentIndex();
    if (!index.isValid()) return;

    mModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void CRM_DocumentWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_DocumentWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`crm_document`.`subject` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter);
}

void CRM_DocumentWidget::on_pbFilterOff_clicked() {
    mModel->setWhere("");
    leFilter->setText("");
}

void CRM_DocumentWidget::on_ileCustomer_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectCustomer);
    CRM_SelectCustomerDialog* customerDlg = dynamic_cast<CRM_SelectCustomerDialog*>(dlg);
    QString projectId = CRM_MODELFACTORY->getRootId();

    if (RB_DEBUG->isValidId(projectId)) {
        customerDlg->setProjectId(projectId);
    }

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("customer_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("customerno").toString() + " "
                            + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No customer selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

void CRM_DocumentWidget::on_ileContact_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectContact);
    CRM_SelectContactDialog* contactDlg = dynamic_cast<CRM_SelectContactDialog*>(dlg);
    int modelRow = mModel->getCurrentIndex().row();
    QModelIndex index = mModel->index(modelRow, mModel->fieldIndex("customer_idx"));
    QString customerId = mModel->data(index, RB2::RoleOrigData).toString();

    if (RB_DEBUG->isValidId(customerId)) {
        contactDlg->setCustomerId(customerId);
    }

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("contact_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("firstname").toString() + " "
                            + obj->getValue("lastname").toString(), Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No contact selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

void CRM_DocumentWidget::on_ileTemplate_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = CRM_DIALOGFACTORY->getDialog(CRM_DialogFactory::DialogSelectTemplate);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("template_idx"));
            mModel->setData(index, obj->getId()
                                  + obj->getValue("reference").toString(), Qt::EditRole);
        }
    } else {
        CRM_DIALOGFACTORY->requestWarningDialog(tr("No template selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String CRM_DocumentWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Close widget
 */
bool CRM_DocumentWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_DocumentWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void CRM_DocumentWidget::preparePrintText(RB_PrintTextHeaderFooter* util,
                                          RB_ObjectBase* docObj) {
    if (!util || !docObj) return;

    QString customerId = mModel->getCurrentValue("customer_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* customerObj = new ACC_Customer(customerId, NULL, "ACC_Customer");
    customerObj->dbRead(CRM_MODELFACTORY->getDatabase());

    QString contactId = mModel->getCurrentValue("contact_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* contactObj = new ACC_Contact(contactId, NULL, "ACC_Contact");
    contactObj->dbRead(CRM_MODELFACTORY->getDatabase());

    QString templateId = mModel->getCurrentValue("template_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* templateObj = new CRM_Template(templateId, NULL, "CRM_Template");
    templateObj->dbRead(CRM_MODELFACTORY->getDatabase());

    util->setPaperMargin(templateObj->getValue("marginleft").toDouble(),
                         templateObj->getValue("margintop").toDouble(),
                         templateObj->getValue("marginright").toDouble(),
                         templateObj->getValue("marginbottom").toDouble());
    util->setPrintPageNumbers(false);
    //    util->setPageOrientation(QPrinter::Portrait);

    QTextDocument* docHeader = new QTextDocument();
    QTextDocument* docContent = new QTextDocument();
    QTextDocument* docFooter = new QTextDocument();
    docHeader->setHtml(templateObj->getValue("pageheader").toString());
    docContent->setHtml(templateObj->getValue("doccontent").toString());
    docFooter->setHtml(templateObj->getValue("pagefooter").toString());

    // append text content
    QTextCursor cursor(docContent);
    cursor.movePosition(QTextCursor::End);
    cursor.insertHtml(docObj->getValue("content").toString());

    util->setDocHeader(docHeader, templateObj->getValue("pageheaderheight").toDouble());
    util->setDocContent(docContent);
    util->setDocFooter(docFooter, templateObj->getValue("pagefooterheight").toDouble());

    util->clearHeaderReplace();
    util->clearContentReplace();
    util->clearFooterReplace();

    util->addContentReplace(QString("{$ACC_Contact.salutation}"), contactObj->getValue("salutation").toString());
    util->addContentReplace(QString("{$ACC_Contact.firstname}"), contactObj->getValue("firstname").toString());
    util->addContentReplace(QString("{$ACC_Contact.lastname}"), contactObj->getValue("lastname").toString());
    util->addContentReplace(QString("{$ACC_Contact.title}"), contactObj->getValue("title").toString());
    util->addContentReplace(QString("{$ACC_Contact.department}"), contactObj->getValue("department").toString());
    util->addContentReplace(QString("{$ACC_Contact.phonework}"), contactObj->getValue("phonework").toString());
    util->addContentReplace(QString("{$ACC_Contact.phonefax}"), contactObj->getValue("phonefax").toString());
    util->addContentReplace(QString("{$ACC_Contact.email1}"), contactObj->getValue("email1").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress1}"), contactObj->getValue("primaryaddress1").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress2}"), contactObj->getValue("primaryaddress2").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress3}"), contactObj->getValue("primaryaddress3").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress4}"), contactObj->getValue("primaryaddress4").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress5}"), contactObj->getValue("primaryaddress5").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress6}"), contactObj->getValue("primaryaddress6").toString());

    util->addContentReplace(QString("{$ACC_Contact.altaddress1}"), contactObj->getValue("altaddress1").toString());
    util->addContentReplace(QString("{$ACC_Contact.altaddress2}"), contactObj->getValue("altaddress2").toString());
    util->addContentReplace(QString("{$ACC_Contact.altaddress3}"), contactObj->getValue("altaddress3").toString());
    util->addContentReplace(QString("{$ACC_Contact.altaddress4}"), contactObj->getValue("altaddress4").toString());
    util->addContentReplace(QString("{$ACC_Contact.altaddress5}"), contactObj->getValue("altaddress5").toString());
    util->addContentReplace(QString("{$ACC_Contact.altaddress6}"), contactObj->getValue("altaddress6").toString());

    util->addContentReplace(QString("{$ACC_Customer.mname}"), customerObj->getValue("mname").toString());
    util->addContentReplace(QString("{$ACC_Customer.customerno}"), customerObj->getValue("customerno").toString());

    util->addContentReplace(QString("{$CRM_Document.reference}"), docObj->getValue("refno").toString());
    util->addContentReplace(QString("{$CRM_Document.subject}"), docObj->getValue("subject").toString());

    util->addContentReplace(QString("{$date}"), QDateTime::currentDateTime().date().toString(Qt::ISODate));

    delete docHeader;
    delete docContent;
    delete docFooter;

    delete templateObj;
    delete contactObj;
    delete customerObj;
}

