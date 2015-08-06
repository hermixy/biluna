/*****************************************************************
 * $Id: srm_campaigntargetwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Mar 5, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_campaigntargetwidget.h"

#include <QStringListModel>
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "srm_objectfactory.h"
#include "srm_printtext.h"
#include "srm_sqlcommonfunctions.h"
#include "db_actionfilepdf.h"
#include "db_actionfileprint.h"
#include "db_actionfileprintpreview.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_printtextheaderfooter.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
SRM_CampaignTargetWidget::SRM_CampaignTargetWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mCampaignModel = NULL;
    mCampaignMapper = NULL;
    mTargetModel = NULL;
    mTargetMapper = NULL;
    mActivityModel = NULL;

    mDocHeader = new QTextDocument();
    mDocContent = new QTextDocument();
    mDocFooter = new QTextDocument();
}

/**
 * Destructor
 */
SRM_CampaignTargetWidget::~SRM_CampaignTargetWidget() {
    if (mTargetModel) {
        delete mTargetModel;
    }

    if (mCampaignModel) {
        delete mCampaignModel;
    }

    // Do not delete mActivityModel because it might be shared with SRM_ActivityWidget
    RB_DEBUG->print("SRM_CampaignTargetWidget::~SRM_CampaignTargetWidget() OK");
}

/**
 * Initialize widget and models
 */
void SRM_CampaignTargetWidget::init() {
    //
    // 1. Set model for campaign mapper
    //
    mCampaignModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelCampaign); // shared
    mCampaignModel->setRoot(SRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mCampaignMapper = mCampaignModel->getMapper();

    RB_StringList items;
    items << tr("Email") << tr("Mail") << tr("Newsletter") << tr("Newspaper")
          << tr("Print") << tr("Radio") << tr("Telesales") << tr("Television");
    cbCampaignType->addItems(items);
    mCampaignMapper->addMapping(cbCampaignType, mCampaignModel->fieldIndex("campaigntype_id"),
                                "currentIndex");
    mCampaignMapper->addMapping(leCode, mCampaignModel->fieldIndex("campaigncode"));
    mCampaignMapper->addMapping(leSubject, mCampaignModel->fieldIndex("subject"));
    mCampaignMapper->addMapping(dteStart, mCampaignModel->fieldIndex("mstart"));
    mCampaignMapper->addMapping(dteEnd, mCampaignModel->fieldIndex("mend"));
    addComboBoxMapping(mCampaignModel, "currency_id", "ACC_Currency", "id",
                       "currcode", cbCurrency, mCampaignMapper);

    mCampaignMapper->addMapping(leBudget, mCampaignModel->fieldIndex("budget"));
    mCampaignMapper->addMapping(leExpectedRevenue, mCampaignModel->fieldIndex("expectedrevenue"));
    items.clear();
    items << tr("Planning") << tr("In Queue") << tr("Sending") << tr("Active")
          << tr("Inactive") << tr("Complete");
    cbStatus->setModel(new QStringListModel(items, this));
    mCampaignMapper->addMapping(cbStatus, mCampaignModel->fieldIndex("campaignstatus_id"),
                                "currentIndex");
    mCampaignMapper->addMapping(pteObjective, mCampaignModel->fieldIndex("objective"));
    mCampaignMapper->addMapping(pteDescription, mCampaignModel->fieldIndex("description"));
    mCampaignMapper->addMapping(ileTemplate, mCampaignModel->fieldIndex("document_idx"));

    // Align and validator
    leBudget->setAlignment(Qt::AlignRight);
    leBudget->setValidator(new QDoubleValidator(0.0, 9999999999.99, 2, this));
    leExpectedRevenue->setAlignment(Qt::AlignRight);
    leExpectedRevenue->setValidator(new QDoubleValidator(0.0, 9999999999.99, 2, this));

    //
    // 3. Select after relations have been set, only for database models
    //
    mCampaignModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvCampaign, mCampaignModel);
    pteObjective->setTabChangesFocus(true);
    pteDescription->setTabChangesFocus(true);
    dteStart->setDisplayFormat("yyyy-MM-dd");
    dteEnd->setDisplayFormat("yyyy-MM-dd");
    connect(dteStart, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateEndDateTime()));

    // Child model and views
    mTargetModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelCampaignTarget); // shared
    mTargetMapper = mTargetModel->getMapper();
    mTargetMapper->addMapping(ileContactGroup, mTargetModel->fieldIndex("group_idx"));
    mTargetMapper->addMapping(leNote, mTargetModel->fieldIndex("note"));
    setFormatTableView(tvTarget, mTargetModel);
    readSettings();

    int count = mCampaignModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mCampaignModel->fieldIndex("campaigncode")
                && i != mCampaignModel->fieldIndex("subject")
                && i != mCampaignModel->fieldIndex("mstart")
                && i != mCampaignModel->fieldIndex("mend")
                && i != mCampaignModel->fieldIndex("budget")
                && i != mCampaignModel->fieldIndex("campaignstatus_id")) {
            tvCampaign->hideColumn(i);
        } else {
            tvCampaign->showColumn(i);

            if (tvCampaign->columnWidth(i) < 5) {
                tvCampaign->setColumnWidth(i, 100);
            }
        }
    }

    count = mTargetModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1) {
            tvTarget->hideColumn(i);
        } else {
            tvTarget->showColumn(i);

            if (tvTarget->columnWidth(i) < 5) {
                tvTarget->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool SRM_CampaignTargetWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mCampaignModel) {
        if (mCampaignModel->submitAllAndSelect()
                && mTargetModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mCampaignModel) {
        if (mCampaignModel->submitAll() && mTargetModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

void SRM_CampaignTargetWidget::filePdf(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr, false);
}

void SRM_CampaignTargetWidget::filePrint(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr, false);
}

void SRM_CampaignTargetWidget::filePrintPreview(QPrinter* pr) {
    if (isWindowModified()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("Please save your data first."));
        return;
    }
    print(pr, true);
}

void SRM_CampaignTargetWidget::print(QPrinter* pr, bool isPreview) {
    if (!tvTarget->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No target group selected."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // fileSave(false);

    // Get document and create list with contacts including customer name,
    // supplier number
    QString targetId = mTargetModel->getCurrentId();
    QSqlQuery query(SRM_MODELFACTORY->getDatabase());

    SRM_SqlCommonFunctions util;
    util.getTemplateId(query, targetId);
    RB_ObjectBase* doc = NULL;

    if (query.next()) {
        doc = SRM_OBJECTFACTORY->newSingleObject("SRM_Template");
        doc->setId(query.value(0).toString());
        doc->dbRead(SRM_MODELFACTORY->getDatabase());
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No campaign document."));
        return;
    }

    query.clear();
    QString groupId = mTargetModel->getCurrentValue("group_idx", RB2::RoleOrigData).toString();
    util.getTargetList(query, groupId);

    RB_ObjectContainer* targetList = new RB_ObjectContainer("", NULL, "SRM_TargetExtendedList");
    setTargetList(targetList, query);

    RB_PrintTextHeaderFooter* utilTxt = new RB_PrintTextHeaderFooter();
    RB_ObjectIterator* iter = targetList->createIterator();

    if (isPreview) {
        iter->first();
        RB_ObjectBase* obj = iter->currentObject();
        preparePrintText(utilTxt, doc, obj);
        utilTxt->print(pr);
    } else {
        int pageNo = 1;
        RB_String fileNameNoExtension = "";

        // Get activity model. Also set root to make sure, select not required
        mActivityModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelActivity, false);
        mActivityModel->setRoot(SRM_MODELFACTORY->getRootId());

        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* obj = iter->currentObject();
            preparePrintText(utilTxt, doc, obj);

            if (pr->outputFormat() == QPrinter::PdfFormat) {
                if (fileNameNoExtension.isEmpty()) {
                    RB_String fileName = pr->outputFileName();
                    fileName.remove(".pdf", Qt::CaseInsensitive);
                    fileNameNoExtension = fileName;
                }

                pr->setOutputFileName(fileNameNoExtension + "_"
                                      + RB_String::number(pageNo) + ".pdf");
                ++pageNo;
            }

            utilTxt->print(pr);

            if (!iter->isDone()) {
                pr->newPage();
            }

            addLogActivity(doc, obj);
        }

        if (mActivityModel) {
            mActivityModel->submitAll(); // no select
        }
    }

    delete iter;
    delete utilTxt;
    delete doc;

    QApplication::restoreOverrideCursor();
}

void SRM_CampaignTargetWidget::addLogActivity(RB_ObjectBase* doc, RB_ObjectBase* target) {
    if (!mActivityModel) return;

    RB_String dt = QDateTime::currentDateTime().date().toString(Qt::ISODate);
    int row = 0;
    mActivityModel->insertRows(row, 1);
    QModelIndex idx;
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitytype_id"));
    mActivityModel->setData(idx, 3); // log
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("activitycode"));
    mActivityModel->setData(idx, target->getValue("suppliercode").toString() + "LOG" + dt);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("subject"));
    mActivityModel->setData(idx, tr("Generated") + " " + doc->getValue("reference").toString());
    dt = QDateTime::currentDateTime().toString(Qt::ISODate);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mstart"));
    mActivityModel->setData(idx, dt);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mend"));
    mActivityModel->setData(idx, dt);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("isreminder"));
    mActivityModel->setData(idx, 0);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("remindertime"));
    mActivityModel->setData(idx, 0);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("priority_id"));
    mActivityModel->setData(idx, 0);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("description"));
    mActivityModel->setData(idx, tr("Generated") + " " + doc->getValue("reference").toString());
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("status_id"));
    mActivityModel->setData(idx, 0);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("location"));
    mActivityModel->setData(idx, tr("None"));
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("mvalue"));
    mActivityModel->setData(idx, 0);
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_contact_id"));
    mActivityModel->setData(idx, target->getId());
    idx = mActivityModel->index(row, mActivityModel->fieldIndex("acc_supplier_id"));
    mActivityModel->setData(idx, target->getValue("parent").toString());
}

/**
 * Button add clicked
 */
void SRM_CampaignTargetWidget::on_pbAddCampaign_clicked() {
    if (!mCampaignModel) return;

    if (isWindowModified()) {
        fileSave(true);
    }

    mCampaignModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mCampaignModel->rowCount();
    mCampaignModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("campaigntype_id"));
    mCampaignModel->setData(idx, 0, Qt::EditRole);
    QDateTime dt = QDateTime::currentDateTime();
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("mstart"));
    mCampaignModel->setData(idx, dt, Qt::EditRole);
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("mend"));
    mCampaignModel->setData(idx, dt, Qt::EditRole);
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("currency_id"));
    mCampaignModel->setData(idx, "0", Qt::EditRole);
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("campaignstatus_id"));
    mCampaignModel->setData(idx, 0, Qt::EditRole);
    idx = mCampaignModel->index(row, mCampaignModel->fieldIndex("document_idx"));
    mCampaignModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    tvCampaign->setCurrentIndex(mCampaignModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvCampaign->scrollTo(tvCampaign->currentIndex());
    cbCampaignType->setFocus();
}

/**
 * Button delete clicked
 */
void SRM_CampaignTargetWidget::on_pbDeleteCampaign_clicked() {
    int ret = SRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete campaign ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this campaign?"));
    if (ret == QMessageBox::No) {
        SRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvCampaign->currentIndex();
    if (!index.isValid()) return;

    mCampaignModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void SRM_CampaignTargetWidget::on_pbSave_clicked() {
    fileSave(true);
}

void SRM_CampaignTargetWidget::on_pbFilterOnCampaign_clicked() {
    RB_String str = leFilterCampaign->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`SRM_campaign`.`campaigncode` LIKE '";
    filter += str + "'";
    mCampaignModel->setWhere(filter);
}

void SRM_CampaignTargetWidget::on_pbFilterOffCampaign_clicked() {
    mCampaignModel->setWhere("");
    leFilterCampaign->setText("");
}

/**
 * Push button select document clicked
 */
void SRM_CampaignTargetWidget::on_ileTemplate_clicked() {
    if (!tvCampaign->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No campaign selected.\n"
                                                   "Please select a campaign first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSelectTemplate);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvCampaign->currentIndex().row();
            QModelIndex index = mCampaignModel->index(row, mCampaignModel->fieldIndex("document_idx"));
            mCampaignModel->setData(index, obj->getId()
                                  + obj->getValue("reference").toString(), Qt::EditRole);
        }
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Document is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Start date time changed, update end date time when before start.
 */
void SRM_CampaignTargetWidget::slotUpdateEndDateTime() {
    QDateTime startDt = dteStart->dateTime();

    if (startDt > dteEnd->dateTime()) {
        dteEnd->setDateTime(startDt);
    }
}

/**
 * Pushbutton Add Target clicked
 */
void SRM_CampaignTargetWidget::on_pbAddTarget_clicked() {
    if (!mCampaignModel || !mTargetModel) return;
    if (!tvCampaign->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No campaign selected.\n"
                                                   "Select/create a campaign first."));
        return;
    }

    mTargetModel->undoFilterSort();

    // always insert at the begin
    int row = 0;
    mTargetModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mTargetModel->index(row, mTargetModel->fieldIndex("group_idx"));
    mTargetModel->setData(idx, "0", Qt::EditRole);
    // end NOTE

    tvTarget->setCurrentIndex(mTargetModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTarget->scrollTo(tvTarget->currentIndex());
    ileContactGroup->setFocus();
}

/**
 * Pushbutton Delete Target clicked
 */
void SRM_CampaignTargetWidget::on_pbDeleteTarget_clicked() {
    int ret = SRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        SRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvTarget->currentIndex();
    if (!index.isValid()) return;

    mTargetModel->removeRows(index.row(), 1, QModelIndex());
    mTargetModel->submitAllAndSelect();
    // setWindowModified(false); parent could have changed
}

/**
 * Pushbutton Filter On Target clicked
 */
void SRM_CampaignTargetWidget::on_pbFilterOnTarget_clicked() {
    RB_String str = leFilterTarget->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    SRM_SqlCommonFunctions util;
    RB_String filter = "`SRM_campaigntarget`.";
    util.substrIdxContent("group_idx", filter);
    filter += " LIKE '";
    filter += str + "'";
    mTargetModel->setWhere(filter);
}

/**
 * Pushbutton Filter Off Target clicked
 */
void SRM_CampaignTargetWidget::on_pbFilterOffTarget_clicked() {
    mTargetModel->setWhere("");
    leFilterTarget->setText("");
}

/**
 * Button select group clicked
 */
void SRM_CampaignTargetWidget::on_ileContactGroup_clicked() {
    if (!tvTarget->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No target selected.\n"
                                                   "Please select a target first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSelectGroup);

    if (dlg && dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvTarget->currentIndex().row();
            QModelIndex index = mTargetModel->index(row, mTargetModel->fieldIndex("group_idx"));
            mTargetModel->setData(index, obj->getId()
                                  + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No Group selected.\n"
                                                   "Group is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Pushbutton Preview clicked
 */
void SRM_CampaignTargetWidget::on_pbPreview_clicked() {
    DB_ActionFilePrintPreview::factory();
}

/**
 * Pushbutton Print clicked
 */
void SRM_CampaignTargetWidget::on_pbPrint_clicked() {
    DB_ActionFilePrint::factory();
}

/**
 * Pushbutton PDF clicked
 */
void SRM_CampaignTargetWidget::on_pbPdf_clicked() {
    DB_ActionFilePdf::factory();
}



/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String SRM_CampaignTargetWidget::getHelpSubject() const {
    return objectName();
}

bool SRM_CampaignTargetWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void SRM_CampaignTargetWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Set the target list with data from the database
 * @param targetList target list to be set
 * @param q database query with target list data
 */
void SRM_CampaignTargetWidget::setTargetList(RB_ObjectContainer* targetList,
                                             QSqlQuery& q) {

    /*
    0 salutation (ACC_Contact)
    1 firstname
    2 lastname
    3 title
    4 department
    5 phonework
    6 phonefax
    7 email1
    8 primaryaddress1
    9 primaryaddress2
    10 primaryaddress3
    11 primaryaddress4
    12 primaryaddress5
    13 primaryaddress6
    14 suppliercode (ACC_Supplier)
    15 suppname (ACC_Supplier)
    16 id (ACC_Contact)
    17 parent (ACC_Contact) is id of ACC_Customer
    */

    while (q.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", targetList, "SRM_TargetExtended");
        targetList->addObject(obj);
        obj->addMember("salutation", "-", q.value(0), RB2::MemberChar125);
        obj->addMember("firstname", "-", q.value(1), RB2::MemberChar125);
        obj->addMember("lastname", "-", q.value(2), RB2::MemberChar125);
        obj->addMember("title", "-", q.value(3), RB2::MemberChar125);
        obj->addMember("department", "-", q.value(4), RB2::MemberChar125);
        obj->addMember("phonework", "-", q.value(5), RB2::MemberChar125);
        obj->addMember("phonefax", "-", q.value(6), RB2::MemberChar125);
        obj->addMember("email1", "-", q.value(7), RB2::MemberChar125);
        obj->addMember("primaryaddress1", "-", q.value(8), RB2::MemberChar125);
        obj->addMember("primaryaddress2", "-", q.value(9), RB2::MemberChar125);
        obj->addMember("primaryaddress3", "-", q.value(10), RB2::MemberChar125);
        obj->addMember("primaryaddress4", "-", q.value(11), RB2::MemberChar125);
        obj->addMember("primaryaddress5", "-", q.value(12), RB2::MemberChar125);
        obj->addMember("primaryaddress6", "-", q.value(13), RB2::MemberChar125);
        obj->addMember("suppliercode", "-", q.value(14), RB2::MemberChar125);
        obj->addMember("suppname", "-", q.value(15), RB2::MemberChar125);
        obj->setId(q.value(16).toString());
        obj->setValue("parent", q.value(17).toString());
    }
}

void SRM_CampaignTargetWidget::preparePrintText(RB_PrintTextHeaderFooter* util,
                                                RB_ObjectBase* docObj,
                                                RB_ObjectBase* targetObj) {
    if (!util || !docObj) return;

    util->setPaperMargin(docObj->getValue("marginleft").toDouble(),
                         docObj->getValue("margintop").toDouble(),
                         docObj->getValue("marginright").toDouble(),
                         docObj->getValue("marginbottom").toDouble());
    util->setPrintPageNumbers(false);
    util->setPageOrientation(QPrinter::Portrait);

    mDocHeader->setHtml(docObj->getValue("pageheader").toString());
    mDocContent->setHtml(docObj->getValue("doccontent").toString());
    mDocFooter->setHtml(docObj->getValue("pagefooter").toString());

    util->setDocHeader(mDocHeader, docObj->getValue("pageheaderheight").toDouble());
    util->setDocContent(mDocContent); // address name
    util->setDocFooter(mDocFooter, docObj->getValue("pagefooterheight").toDouble());

    util->clearHeaderReplace();
    util->clearContentReplace(); // changed below with targetObj data
    util->clearFooterReplace();

    if (!targetObj) return;

    util->addContentReplace(QString("{$ACC_Contact.salutation}"), targetObj->getValue("salutation").toString());
    util->addContentReplace(QString("{$ACC_Contact.firstname}"), targetObj->getValue("firstname").toString());
    util->addContentReplace(QString("{$ACC_Contact.lastname}"), targetObj->getValue("lastname").toString());
    util->addContentReplace(QString("{$ACC_Contact.title}"), targetObj->getValue("title").toString());
    util->addContentReplace(QString("{$ACC_Contact.department}"), targetObj->getValue("department").toString());
    util->addContentReplace(QString("{$ACC_Contact.phonework}"), targetObj->getValue("phonework").toString());
    util->addContentReplace(QString("{$ACC_Contact.phonefax}"), targetObj->getValue("phonefax").toString());
    util->addContentReplace(QString("{$ACC_Contact.email1}"), targetObj->getValue("email1").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress1}"), targetObj->getValue("primaryaddress1").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress2}"), targetObj->getValue("primaryaddress2").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress3}"), targetObj->getValue("primaryaddress3").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress4}"), targetObj->getValue("primaryaddress4").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress5}"), targetObj->getValue("primaryaddress5").toString());
    util->addContentReplace(QString("{$ACC_Contact.primaryaddress6}"), targetObj->getValue("primaryaddress6").toString());

    util->addContentReplace(QString("{$ACC_Supplier.suppname}"), targetObj->getValue("suppname").toString());
    util->addContentReplace(QString("{$ACC_Supplier.suppliercode}"), targetObj->getValue("suppliercode").toString());

    util->addContentReplace(QString("{$SRM_Template.reference}"), docObj->getValue("reference").toString());

    util->addContentReplace(QString("{$date}"), QDateTime::currentDateTime().date().toString(Qt::ISODate));
}
