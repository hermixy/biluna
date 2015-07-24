/*****************************************************************
 * $Id: acc_pricewidget.cpp 2144 2014-05-01 18:22:24Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_pricewidget.h"

#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_PriceWidget::ACC_PriceWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mParentModel = NULL;
    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_PriceWidget::~ACC_PriceWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel;
    RB_DEBUG->print("ACC_PriceWidget::~ACC_PriceWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_PriceWidget::init() {
    // Set parent model, stockmaster, for name only
    mParentModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockMaster);
    connect(mParentModel, SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotParentSelectionChanged(QModelIndex,QModelIndex)));

    // parent will be set by selecting the stock item in ACC_StockMasterWidget
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPrice);

    mMapper = mModel->getMapper();
    addComboBoxMapping(mModel, "salestype_id", "ACC_SalesType",
                       "id", "typeabbrev", cbSalesType, mMapper);
    addComboBoxMapping(mModel, "currency_id", "ACC_Currency",
                       "id", "currency", cbCurrency, mMapper);
    mMapper->addMapping(leSalesPrice, mModel->fieldIndex("price"));
    leSalesPrice->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));

    setFormatTableView(tvPrice, mModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvPrice->hideColumn(i);
        } else {
            tvPrice->showColumn(i);

            if (tvPrice->columnWidth(i) < 5) {
                tvPrice->setColumnWidth(i, 100);
            }
        }
    }

    // In case the navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockMaster);
    mModel->slotParentCurrentRowChanged(idx, QModelIndex());
    slotParentSelectionChanged(idx, QModelIndex());
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAllAndSelect()
 */
bool ACC_PriceWidget::fileSave(bool /*withSelect*/) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();

    if (mModel->submitAllAndSelect()) {

        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    }

    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void ACC_PriceWidget::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

/**
 * Button add clicked
 */
void ACC_PriceWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("currency_id"));
    mModel->setData(index, "0", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("salestype_id"));
    mModel->setData(index, "0", Qt::EditRole);
    // end NOTE

    tvPrice->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS));
    tvPrice->scrollTo(tvPrice->currentIndex());
    cbSalesType->setFocus();
}

/**
 * Button delete clicked
 */
void ACC_PriceWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvPrice->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        mModel->submitAllAndSelect();
        setWindowModified(false);
    }
}

/**
 * Button save clicked
 */
void ACC_PriceWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Slot to set name of supplier
 */
void ACC_PriceWidget::slotParentSelectionChanged(const QModelIndex& current,
                                                       const QModelIndex& /*previous*/) {
    RB_String str = mParentModel->data(mParentModel->index(current.row(),
                                                           mParentModel->fieldIndex("stockcode"),
                                                           current.parent())).toString() + " - "
            + mParentModel->data(mParentModel->index(current.row(),
                                         mParentModel->fieldIndex("description"),
                                         current.parent())).toString();
    leItem->setText(str);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_PriceWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_PriceWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void ACC_PriceWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
