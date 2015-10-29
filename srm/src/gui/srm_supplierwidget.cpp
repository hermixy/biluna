/*****************************************************************
 * $Id: srm_supplierwidget.cpp 2241 2015-05-22 10:22:19Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_supplierwidget.h"

#include <QStringListModel>
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "srm_selectprojectdialog.h"
#include "db_dialogfactory.h"
#include "db_internetbrowserfactory.h"
#include "db_internetbrowserwidget.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
SRM_SupplierWidget::SRM_SupplierWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mSuppModel = NULL;
    mSuppMapper = NULL;
    mSuppDetModel = NULL;
    mSuppDetMapper = NULL;
    mItemModel = NULL;
    mItemMapper = NULL;
}

/**
 * Destructor
 */
SRM_SupplierWidget::~SRM_SupplierWidget() {
    delete mSuppModel;
    delete mSuppDetModel;
    delete mItemModel;
}

/**
 * Initialize widget and models
 */
void SRM_SupplierWidget::init() {
    //
    // 1. Set model for supplier mapper
    //
    mSuppModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSupplier); // shared
    mSuppModel->setRoot(SRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mSuppMapper = mSuppModel->getMapper();
    mSuppMapper->addMapping(leSupplierCode, mSuppModel->fieldIndex("suppliercode"));
    mSuppMapper->addMapping(leSupplierName, mSuppModel->fieldIndex("suppname"));
    mSuppMapper->addMapping(leAddressLine1, mSuppModel->fieldIndex("address1"));
    mSuppMapper->addMapping(leAddressLine2, mSuppModel->fieldIndex("address2"));
    mSuppMapper->addMapping(leAddressLine3, mSuppModel->fieldIndex("address3"));
    mSuppMapper->addMapping(leAddressLine4, mSuppModel->fieldIndex("address4"));
    mSuppMapper->addMapping(leAddressLine5, mSuppModel->fieldIndex("address5"));
    mSuppMapper->addMapping(leAddressLine6, mSuppModel->fieldIndex("address6"));
    mSuppMapper->addMapping(leSupplierContact, mSuppModel->fieldIndex("contactname"));
    mSuppMapper->addMapping(leEmail, mSuppModel->fieldIndex("email"));
    mSuppMapper->addMapping(lePhoneNumber, mSuppModel->fieldIndex("phoneno"));
    mSuppMapper->addMapping(leFaxNumber, mSuppModel->fieldIndex("faxno"));

    addComboBoxMapping(mSuppModel, "db_systemuser_id", "DB_SystemUser", "id", "username",
                       cbInternalAccountHolder, mSuppMapper);
    QStringList items;
    items.clear();
    items << tr("No") << tr("Yes");
    cbExistingSupplier->setModel(new QStringListModel(items, this));
    mSuppMapper->addMapping(cbExistingSupplier, mSuppModel->fieldIndex("srm_type_id"),
                            "currentIndex");

    //
    // 3. Select after relations have been set, only for database models
    //
    mSuppModel->select();
//    mSuppModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
//    mSuppModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvSupplier, mSuppModel);

    int count = mSuppModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        // stockmaster_idx not set in supplier relation management
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1
                && i != mSuppModel->fieldIndex("contactname")
                && i != mSuppModel->fieldIndex("phoneno")
                && i != mSuppModel->fieldIndex("email")) {
            tvSupplier->hideColumn(i);
        } else {
            tvSupplier->showColumn(i);

            if (tvSupplier->columnWidth(i) < 5) {
                tvSupplier->setColumnWidth(i, 100);
            }
        }
    }

    // Detail model
    mSuppDetModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelSupplierDetail); // shared
    mSuppDetMapper = mSuppDetModel->getMapper();

    items.clear();
    items << "Other" << "Analyst" << "Competitor" << "Integrator" << "Investor"
          << "Partner" << "Press" << "Prospect" << "Reseller" << "Supplier";
    cbType->setModel(new QStringListModel(items, this));
    mSuppDetMapper->addMapping(cbType, mSuppDetModel->fieldIndex("suppliertype_id"),
                               "currentIndex");
    mSuppDetMapper->addMapping(this->leOwnership, mSuppDetModel->fieldIndex("ownership"));
    mSuppDetMapper->addMapping(this->leClassificationCode, mSuppDetModel->fieldIndex("classifcode"));

    items.clear();
    items  << "Other" << "Apparel" << "Banking" << "Biotechnology" << "Chemicals"
          << "Communications" << "Construction" << "Consulting"
          << "Education" << "Electronics" << "Energy" << "Engineering"
          << "Entertainment" << "Environmental" << "Finance"
          << "Government" << "Healthcare" << "Hospitality" << "Insurance"
          << "Machinery" << "Manufacturing" << "Media" << "Not For Profit"
          << "Recreation" << "Retail" << "Shipping" << "Technology"
          << "Telecommunications" << "Transportation" << "Utilities";
    cbIndustry->setModel(new QStringListModel(items, this));
    mSuppDetMapper->addMapping(cbIndustry, mSuppDetModel->fieldIndex("industrytype_id"),
                               "currentIndex");
    mSuppDetMapper->addMapping(this->leNoEmployees, mSuppDetModel->fieldIndex("employeecount"));
    mSuppDetMapper->addMapping(this->leAnnualRevenue, mSuppDetModel->fieldIndex("annualrevenue"));
    mSuppDetMapper->addMapping(this->leRating, mSuppDetModel->fieldIndex("rating"));

    // Add row if not exists
    connect(mSuppModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotParentRowChanged(QModelIndex,QModelIndex)));
    // Because mSuppDetModel has no setFormatTableView()
    connect(mSuppDetModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex, QModelIndex)));

    //
    // 1. Set child model
    //
    mItemModel = SRM_MODELFACTORY->getModel(SRM_ModelFactory::ModelPurchData);

    //
    // 2. Mapper
    //
    mItemMapper = mItemModel->getMapper();
    // mItemMapper->addMapping(leStockCode, mItemModel->fieldIndex("stockmaster_idx"));
    mItemMapper->addMapping(leItemCode, mItemModel->fieldIndex("supplier_partno"));
    mItemMapper->addMapping(leItemDescription, mItemModel->fieldIndex("supplierdescription"));
    mItemMapper->addMapping(lePrice, mItemModel->fieldIndex("price"));
    mItemMapper->addMapping(leUom, mItemModel->fieldIndex("supplieruom"));
    mItemMapper->addMapping(leConversionFactor, mItemModel->fieldIndex("conversionfactor"));
    mItemMapper->addMapping(leLeadTime, mItemModel->fieldIndex("leadtime"));
    items.clear();
    items << tr("No") << tr("Yes");
    cbPreferredSupplier->setModel(new QStringListModel(items, this));
    mItemMapper->addMapping(cbPreferredSupplier, mItemModel->fieldIndex("preferred"), "currentIndex");
    mItemMapper->addMapping(dteEffectiveFrom, mItemModel->fieldIndex("effectivefrom"));

    // 3. select by parent selection

    //
    // 4. Connect model to view
    //
    setFormatTableView(tvItems, mItemModel);

    count = mItemModel->columnCount();

    for (int i = 0; i < count; ++i) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS + 1
                && i != RB2::HIDDENCOLUMNS + 2 && i != RB2::HIDDENCOLUMNS + 3) {
            tvItems->hideColumn(i);
        } else {
            tvItems->showColumn(i);

            if (tvItems->columnWidth(i) < 5) {
                tvItems->setColumnWidth(i, 100);
            }
        }
    }
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool SRM_SupplierWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mSuppModel && mItemModel) {
        if (mSuppModel->submitAllAndSelect()
                && mItemModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mSuppModel && mItemModel) {
        if (mSuppModel->submitAll()
                && mItemModel->submitAll()) {
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
void SRM_SupplierWidget::on_pbAdd_clicked() {
    if (!mSuppModel) return;
    mSuppModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mSuppModel->rowCount();
    mSuppModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index;
    index = mSuppModel->index(row, mSuppModel->fieldIndex("suppliercode"));
    mSuppModel->setData(index, tr("<NEW>"), Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("currency_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mSuppModel->index(row, mSuppModel->fieldIndex("suppliersince"));
    mSuppModel->setData(index, date, Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("paymentterm_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("remittance"));
    mSuppModel->setData(index, 0, Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("taxgroup_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("factorcompany_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);
    index = mSuppModel->index(row, mSuppModel->fieldIndex("area_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);

    index = mSuppModel->index(row, mSuppModel->fieldIndex("srm_type_id"));
    mSuppModel->setData(index, 0, Qt::EditRole); // 0 = potential ACC supplier
    index = mSuppModel->index(row, mSuppModel->fieldIndex("db_systemuser_id"));
    mSuppModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvSupplier->setCurrentIndex(mSuppModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvSupplier->scrollTo(tvSupplier->currentIndex());
    tabWidget->setCurrentIndex(0);
    leSupplierCode->setFocus();
    leSupplierCode->selectAll();
}

/**
 * Button delete clicked
 */
void SRM_SupplierWidget::on_pbDelete_clicked() {
    int ret = DB_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete contact ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this contact?"));
    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvSupplier->currentIndex();
    if (!index.isValid()) return;

    mSuppModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void SRM_SupplierWidget::on_pbSave_clicked() {
    fileSave(true);
}

void SRM_SupplierWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`acc_supplier`.`suppliercode` LIKE '";
    filter += str + "'";
    mSuppModel->setWhere(filter, true);
}

void SRM_SupplierWidget::on_pbFilterOff_clicked() {
    mSuppModel->setWhere("", true);
}

/**
 * Push button map location clicked,
 * Uses 'http://maps.google.com/maps?q=1683%20Mass%20Ave,%20Cambridge,%20MA'
 * the %20 are spaces
 */
void SRM_SupplierWidget::on_pbMapLocation_clicked() {
    if (!mSuppModel->getProxyIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Please select a supplier first."));
        return;
    }

    DB_InternetBrowserWidget* wdgt = DB_INTERNETBROWSERFACTORY->newBrowserWidget();
    QString strUrl = "http://maps.google.com/maps?q=";

    if (!leAddressLine1->text().isEmpty()) {
        strUrl.append(leAddressLine1->text().trimmed() + " ");
    }
    if (!leAddressLine2->text().isEmpty()) {
        strUrl.append(leAddressLine2->text().trimmed() + " ");
    }
    if (!leAddressLine3->text().isEmpty()) {
        strUrl.append(leAddressLine3->text().trimmed() + " ");
    }
    if (!leAddressLine4->text().isEmpty()) {
        strUrl.append(leAddressLine4->text().trimmed() + " ");
    }
    if (!leAddressLine5->text().isEmpty()) {
        strUrl.append(leAddressLine5->text().trimmed() + " ");
    }
    if (!leAddressLine6->text().isEmpty()) {
        strUrl.append(leAddressLine6->text().trimmed() + " ");
    }

    wdgt->loadPage(strUrl);
}

/**
 * Push button send email clicked
 */
void SRM_SupplierWidget::on_pbSendEmail_clicked() {
    RB_String email = leEmail->text();
    if (email.isEmpty()) {
        return;
    }
    QDesktopServices::openUrl(QUrl("mailto:" + email + "?subject=<Subject>&body=Dear ,"));
}

/**
 * Push button add (stock) item clicked
 */
void SRM_SupplierWidget::on_pbAddItem_clicked() {
    if (!mItemModel) return;
    mItemModel->undoFilterSort();

    // always insert at the end
    int row = mItemModel->rowCount();
    mItemModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index;
    index = mItemModel->index(row, mItemModel->fieldIndex("stockmaster_idx"));
    mItemModel->setData(index, "0", Qt::EditRole);
    index = mItemModel->index(row, mItemModel->fieldIndex("price"));
    mItemModel->setData(index, 0.0, Qt::EditRole);
    index = mItemModel->index(row, mItemModel->fieldIndex("supplieruom"));
    mItemModel->setData(index, "-", Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mItemModel->index(row, mItemModel->fieldIndex("effectivefrom"));
    mItemModel->setData(index, date, Qt::EditRole);
    // end NOTE

    tvItems->setCurrentIndex(mItemModel->index(row, RB2::HIDDENCOLUMNS));
    tvItems->scrollTo(tvItems->currentIndex());
}

/**
 * Push button delete (stock) item clicked
 */
void SRM_SupplierWidget::on_pbDeleteItem_clicked() {
    if (!mItemModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvItems->currentIndex();
    if (index.isValid()) {
        mItemModel->removeRows(index.row(), 1, QModelIndex());
        mItemModel->submitAll();
    }
}

void SRM_SupplierWidget::on_pbSelectCompany_clicked() {
    if (!tvSupplier->currentIndex().isValid()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogProject);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mSuppModel->getProxyIndex();
            QModelIndex idx = mSuppModel->index(
                        index.row(), mSuppModel->fieldIndex("SRM_parent"));
            mSuppModel->setData(idx, obj->getId());
        }
    } else {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Parent row changed in ACC_Supplier. Timer
 * will fire single shot after 200 milliseconds to activate
 * slotHandleParentRowChanged(). This delay is set to prevent many SQL to
 * get from related table(s) or to create
 * rows that are only browsed very quickly by the user.
 *
 * Add row in related model(s) (SRM_SupplierDetail) if not exists and
 * set QDataWidgetMapper to relevant index.
 *
 * @param curr current model index not used
 * @param prev previous model index not used
 */
void SRM_SupplierWidget::slotParentRowChanged(const QModelIndex& /*curr*/,
                                               const QModelIndex& /*prev*/) {
    mCurrentId = mSuppModel->getCurrentId();
    QTimer::singleShot(200, this, SLOT(slotHandleParentRowChanged()));
}

/**
 * Add row in related model(s) (SRM_SupplierDetail) if not exists. The
 * current ID needs to be the same as the set ID at slotAddRowIfNotExists().
 * Otherwise the user has apparently already selected a different row.
 */
void SRM_SupplierWidget::slotHandleParentRowChanged() {
    if (mCurrentId != mSuppModel->getCurrentId()) {
        // User already selected another row
        return;
    }

    QModelIndex idx;

    if (mSuppDetModel->rowCount() > 0) {
        // Row already exists in model
        idx = mSuppDetModel->index(0, 0);
        mSuppDetModel->slotChangeCurrentRow(idx, QModelIndex());
        return;
    }

    // always insert at the begin
    int row = 0; // mSuppModel->rowCount();
    mSuppDetModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    idx = mSuppDetModel->index(row, mSuppDetModel->fieldIndex("suppliertype_id"));
    mSuppDetModel->setData(idx, 3, Qt::EditRole);
    idx = mSuppDetModel->index(row, mSuppDetModel->fieldIndex("industrytype_id"));
    mSuppDetModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    mSuppDetModel->submitAll();
    idx = mSuppDetModel->index(0, 0);
    mSuppDetModel->slotChangeCurrentRow(idx, QModelIndex());
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String SRM_SupplierWidget::getHelpSubject() const {
    return objectName();
}

bool SRM_SupplierWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

QModelIndex SRM_SupplierWidget::getCurrentModelIndex(int modelType) const {
    if (modelType == SRM_ModelFactory::ModelSupplier) {
        return tvSupplier->currentIndex();
    }

    return QModelIndex();
}

/**
 * Change event, for example translation
 */
void SRM_SupplierWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
