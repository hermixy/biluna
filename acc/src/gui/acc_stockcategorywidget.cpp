/*****************************************************************
 * $Id: acc_stockcategorywidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 16, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockcategorywidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_selectchartmasterdialog.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_StockCategoryWidget::ACC_StockCategoryWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_StockCategoryWidget::~ACC_StockCategoryWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mModel; not here deleted by ACC_DebtorsMasterWidget (dockwindow)

    // HACK: otherwise crash (BTW do not use delete mMapper)
    mModel->deleteMapper();

    RB_DEBUG->print("ACC_StockCategoryWidget::~ACC_StockCategoryWidget() OK");
}

/**
 * Initialize widget and models
 * TODO: maybeSave() is not handled yet
 */
void ACC_StockCategoryWidget::init() {
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockCategory);

    // because no setFormatTableView()
    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
    connect(mModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));

    mMapper = mModel->getMapper();
    mMapper->addMapping(leCategoryCode, mModel->fieldIndex("categorycode"));
    mMapper->addMapping(leDescription, mModel->fieldIndex("categorydescription"));
    RB_StringList items;
    items << tr("Non-stock") << tr("Labor stock item") << tr("Finished product")
          << tr("Raw material");
    cbStockType->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbStockType, mModel->fieldIndex("stocktype"), "currentIndex");
    addComboBoxMapping(mModel, "taxcat_id", "ACC_TaxCategory", "id", "taxcatname",
                       cbTaxCategory, mMapper);
    mMapper->addMapping(ileStockGlCode,
                        mModel->fieldIndex("stockact_idx"));
    mMapper->addMapping(ileStockAdjustmentGlCode,
                        mModel->fieldIndex("adjglact_idx"));
    mMapper->addMapping(ilePriceVarianceGlCode,
                        mModel->fieldIndex("purchpricevaract_idx"));
    mMapper->addMapping(ileUsageVarianceGlCode,
                        mModel->fieldIndex("materialuseagevarac_idx"));
    mMapper->addMapping(ileWipGlCode,
                        mModel->fieldIndex("wipact_idx"));

    readSettings();

    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockCategory);
    mMapper->setCurrentModelIndex(idx);
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_StockCategoryWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    bool parentSuccess = false;

    if (withSelect) {
        parentSuccess = mModel->submitAllAndSelect();

    } else {
        parentSuccess = mModel->submitAll();
    }
    // QApplication::restoreOverrideCursor();

    if (!parentSuccess) {
        return false;
    }

    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_StockCategoryWidget::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}


/**
 * Button save clicked
 */
void ACC_StockCategoryWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button revert clicked
 */
void ACC_StockCategoryWidget::on_pbRevert_clicked() {
    fileRevert();
}

void ACC_StockCategoryWidget::on_ileStockGlCode_clicked() {
    setAccount("stockact_idx");
}

void ACC_StockCategoryWidget::on_ileWipGlCode_clicked() {
    setAccount("wipact_idx");
}

void ACC_StockCategoryWidget::on_ileStockAdjustmentGlCode_clicked() {
    setAccount("adjglact_idx");
}

void ACC_StockCategoryWidget::on_ilePriceVarianceGlCode_clicked() {
    setAccount("purchpricevaract_idx");
}

void ACC_StockCategoryWidget::on_ileUsageVarianceGlCode_clicked() {
    setAccount("materialuseagevarac_idx");
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void ACC_StockCategoryWidget::slotResetWidgets() {
    leCategoryCode->setFocus();
    leCategoryCode->selectAll();
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_StockCategoryWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_StockCategoryWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_StockCategoryWidget::changeEvent(QEvent *e) {
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
 * Set selected account
 * @param fieldName field to be set
 * @param obj object with account data
 */
void ACC_StockCategoryWidget::setAccount(const QString& fieldName) {
    ACC_SelectChartMasterDialog* dlg = new ACC_SelectChartMasterDialog(this);
    dlg->init();

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            QModelIndex index = mModel->getProxyIndex();
            QModelIndex idx = mModel->index(index.row(),
                                            mModel->fieldIndex(fieldName),
                                            index.parent());

            mModel->setData(idx, obj->getId()
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());
        }
    }

    dlg->deleteLater();
}
