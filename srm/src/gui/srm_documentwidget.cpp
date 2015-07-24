/*****************************************************************
 * $Id: srm_documentwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 25, 2014 3:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_documentwidget.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "srm_selectcontactdialog.h"
#include "srm_selectsupplierdialog.h"
#include "srm_sqlcommonfunctions.h"
#include "srm_template.h"
#include "acc_contact.h"
#include "acc_supplier.h"
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_printtextheaderfooter.h"
#include "rb_sqlrelationaldelegate.h"



/**
 * Constructor
 */
SRM_DocumentWidget::SRM_DocumentWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
SRM_DocumentWidget::~SRM_DocumentWidget() {
    delete mModel; // do not forget to delete otherwise application crashes
    RB_DEBUG->print("SRM_DocumentWidget::~SRM_DocumentWidget() OK");
}

/**
 * Get proposed saveas filename
 * @returns filename
 */
RB_String SRM_DocumentWidget::getSaveAsFileName() {
    if (!mModel->getCurrentIndex().isValid()) {
        return QString();
    }

    RB_String str = mModel->getCurrentValue("refno").toString();
    return str;
}

/**
 * Initialize widget and models
 */
void SRM_DocumentWidget::init() {

    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    //
    // 1. Set model for customer mapper
    //
    mModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelDocument, false);
    mModel->setRoot(SRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    mMapper->addMapping(sbRefNo, mModel->fieldIndex("refno"));
    mMapper->addMapping(leSubject, mModel->fieldIndex("subject"));
    mMapper->addMapping(ileSupplier, mModel->fieldIndex("supplier_idx"));
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
                && i != mModel->fieldIndex("supplier_idx")
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
bool SRM_DocumentWidget::fileSave(bool withSelect) {
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

void SRM_DocumentWidget::filePdf(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr);
}

void SRM_DocumentWidget::filePrint(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr);
}

void SRM_DocumentWidget::filePrintPreview(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr, true);
}

/**
 * Edit Cut.
 */
void SRM_DocumentWidget::editCut() {
    teContent->cut();
}

/**
 * Edit Copy.
 */
void SRM_DocumentWidget::editCopy() {
    teContent->copy();
}

/**
 * Edit Paste.
 */
void SRM_DocumentWidget::editPaste() {
    teContent->paste();
}

/**
 * Edit Undo.
 */
void SRM_DocumentWidget::editUndo() {
    teContent->undo();
}

/**
 * Edit Redo.
 */
void SRM_DocumentWidget::editRedo() {
    teContent->redo();
}

/**
 * Select All.
 */
void SRM_DocumentWidget::editSelectAll() {
    teContent->selectAll();
}

/**
 * Format Bold
 */
void SRM_DocumentWidget::formatBold() {
    teContent->slotTextBold();
}

/**
 * Format Italic
 */
void SRM_DocumentWidget::formatItalic() {
    teContent->slotTextItalic();
}

/**
 * Format Underline
 */
void SRM_DocumentWidget::formatUnderline() {
    teContent->slotTextUnderline();
}

/**
 * Format Strikethrough
 */
void SRM_DocumentWidget::formatStrikethrough() {
    teContent->slotTextStrikeOut();
}

/**
 * Format Align Left
 */
void SRM_DocumentWidget::formatAlignLeft() {
    teContent->slotTextAlign(RB_TextEdit::AlignLeft);
}

/**
 * Format Align Center
 */
void SRM_DocumentWidget::formatAlignCenter() {
    teContent->slotTextAlign(RB_TextEdit::AlignCenter);
}

/**
 * Format Align Right
 */
void SRM_DocumentWidget::formatAlignRight() {
    teContent->slotTextAlign(RB_TextEdit::AlignRight);
}

/**
 * Format Align Justify
 */
void SRM_DocumentWidget::formatAlignJustify() {
    teContent->slotTextAlign(RB_TextEdit::AlignJustify);
}

/**
 * Format Increase Indent
 */
void SRM_DocumentWidget::formatIncreaseIndent() {
    teContent->slotTextIndent(1);
}

/**
 * Format Decrease Indent
 */
void SRM_DocumentWidget::formatDecreaseIndent() {
    teContent->slotTextIndent(-1);
}

/**
 * Format numbered list
 */
void SRM_DocumentWidget::formatNumberedList() {
    teContent->slotTextStyle(RB_TextEdit::ListDecimal);
}

/**
 * Format bulleted list
 */
void SRM_DocumentWidget::formatBulletedList() {
    teContent->slotTextStyle(RB_TextEdit::ListDisc);
}

/**
 * Format font family
 */
void SRM_DocumentWidget::formatFontName() {
    teContent->slotTextFamily();
}

/**
 * Format font size
 */
void SRM_DocumentWidget::formatFontSize() {
    teContent->slotTextSize();
}

/**
 * Format text color
 */
void SRM_DocumentWidget::formatTextColor() {
    teContent->slotTextColor();
}

/**
 * Format background color
 */
void SRM_DocumentWidget::formatBackgroundColor() {
    teContent->slotBackgroundColor();
}

/**
 * Remove formatting and style
 */
void SRM_DocumentWidget::formatRemove() {
    //    webView->triggerPageAction(QWebPage::RemoveFormat);
}

void SRM_DocumentWidget::print(QPrinter* pr, bool isPreview) {
    if (!tvDocument->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected."));
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

void SRM_DocumentWidget::addLogActivity(RB_ObjectBase* docObj) {
    RB_MmProxy* activityModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelActivity);
    if (!activityModel) return;

    RB_String dt = QDateTime::currentDateTime().date().toString(Qt::ISODate);
    int row = 0;
    activityModel->insertRows(row, 1);

    QModelIndex idx;
    idx = activityModel->index(row, activityModel->fieldIndex("activitytype_id"));
    activityModel->setData(idx, 3); // log

    int modelRow = mModel->getCurrentIndex().row();
    QModelIndex index = mModel->index(modelRow, mModel->fieldIndex("supplier_idx"));
    QString supplierCodeName = mModel->data(index).toString();
    QStringList supplierList = supplierCodeName.split(" ");
    idx = activityModel->index(row, activityModel->fieldIndex("activitycode"));
    activityModel->setData(idx, supplierList.at(0) + "LOG" + dt);

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

    index = mModel->index(modelRow, mModel->fieldIndex("supplier_idx"));
    QString supplierId = mModel->data(index, RB2::RoleOrigData).toString();
    idx = activityModel->index(row, activityModel->fieldIndex("acc_supplier_id"));
    activityModel->setData(idx, supplierId);

}

/**
 * Button add clicked
 */
void SRM_DocumentWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // Set supplier if already selected
    if (SRM_DIALOGFACTORY->isWidgetActive(SRM_DialogFactory::WidgetSupplier)) {
        RB_Widget* wgt = SRM_DIALOGFACTORY->getWidget(SRM_DialogFactory::WidgetSupplier, NULL);
        QModelIndex index = wgt->getCurrentModelIndex(SRM_ModelFactory::ModelSupplier);

        if (index.isValid()) {
            RB_MmProxy* model = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSupplier);
            index = model->index(index.row(), model->fieldIndex("id"));
            QString uuid = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("suppliercode"));
            QString code = index.data(Qt::EditRole).toString();
            index = model->index(index.row(), model->fieldIndex("suppname"));
            QString name = index.data(Qt::EditRole).toString();

            index = mModel->index(row, mModel->fieldIndex("supplier_idx"));
            mModel->setData(index, uuid + code + " " + name, Qt::EditRole);
        }
    }

    // Set contact if already selected
    if (SRM_DIALOGFACTORY->isWidgetActive(SRM_DialogFactory::WidgetContact)) {
        RB_Widget* wgt = SRM_DIALOGFACTORY->getWidget(SRM_DialogFactory::WidgetContact, NULL);
        QModelIndex index = wgt->getCurrentModelIndex(SRM_ModelFactory::ModelContact);

        if (index.isValid()) {
            RB_MmProxy* model = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelContact);
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

    // number, set (subject) and (do not select contact if not part of supplier if supplier selected)
    SRM_SqlCommonFunctions oper;
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
void SRM_DocumentWidget::on_pbDelete_clicked() {
    int ret = SRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete document ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this document?"));
    if (ret == QMessageBox::No) {
        SRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
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
void SRM_DocumentWidget::on_pbSave_clicked() {
    fileSave(true);
}

void SRM_DocumentWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`srm_document`.`subject` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter);
}

void SRM_DocumentWidget::on_pbFilterOff_clicked() {
    mModel->setWhere("");
    leFilter->setText("");
}

void SRM_DocumentWidget::on_ileSupplier_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSelectSupplier);
    SRM_SelectSupplierDialog* supplierDlg = dynamic_cast<SRM_SelectSupplierDialog*>(dlg);
    QString projectId = SRM_MODELFACTORY->getRootId();

    if (RB_DEBUG->isValidId(projectId)) {
        supplierDlg->setProjectId(projectId);
    }

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("supplier_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("suppliercode").toString() + " "
                            + obj->getValue("suppname").toString(), Qt::EditRole);
        }
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

void SRM_DocumentWidget::on_ileContact_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSelectContact);
    SRM_SelectContactDialog* contactDlg = dynamic_cast<SRM_SelectContactDialog*>(dlg);
    int modelRow = mModel->getCurrentIndex().row();
    QModelIndex index = mModel->index(modelRow, mModel->fieldIndex("supplier_idx"));
    QString supplierId = mModel->data(index, RB2::RoleOrigData).toString();

    if (RB_DEBUG->isValidId(supplierId)) {
        contactDlg->setSupplierId(supplierId);
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
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No contact selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

void SRM_DocumentWidget::on_ileTemplate_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSelectTemplate);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mModel->index(row, mModel->fieldIndex("template_idx"));
            mModel->setData(index, obj->getId()
                                  + obj->getValue("reference").toString(), Qt::EditRole);
        }
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No template selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String SRM_DocumentWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Close widget
 */
bool SRM_DocumentWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void SRM_DocumentWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void SRM_DocumentWidget::preparePrintText(RB_PrintTextHeaderFooter* util,
                                          RB_ObjectBase* docObj) {
    if (!util || !docObj) return;

    QString supplierId = mModel->getCurrentValue("supplier_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* supplierObj = new ACC_Supplier(supplierId, NULL, "ACC_Supplier");
    supplierObj->dbRead(SRM_MODELFACTORY->getDatabase());

    QString contactId = mModel->getCurrentValue("contact_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* contactObj = new ACC_Contact(contactId, NULL, "ACC_Contact");
    contactObj->dbRead(SRM_MODELFACTORY->getDatabase());

    QString templateId = mModel->getCurrentValue("template_idx", RB2::RoleOrigData).toString();
    RB_ObjectBase* templateObj = new SRM_Template(templateId, NULL, "SRM_Template");
    templateObj->dbRead(SRM_MODELFACTORY->getDatabase());

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

    util->addContentReplace(QString("{$ACC_Supplier.suppname}"), supplierObj->getValue("suppname").toString());
    util->addContentReplace(QString("{$ACC_Supplier.suppliercode}"), supplierObj->getValue("suppliercode").toString());

    util->addContentReplace(QString("{$SRM_Document.reference}"), docObj->getValue("refno").toString());
    util->addContentReplace(QString("{$SRM_Document.subject}"), docObj->getValue("subject").toString());

    util->addContentReplace(QString("{$date}"), QDateTime::currentDateTime().date().toString(Qt::ISODate));

    delete docHeader;
    delete docContent;
    delete docFooter;

    delete templateObj;
    delete contactObj;
    delete supplierObj;
}
