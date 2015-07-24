/*****************************************************************
 * $Id: acc_assetmasterwidget.cpp 2010 2013-10-04 20:58:40Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetmasterwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelation.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_AssetMasterWidget::ACC_AssetMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
ACC_AssetMasterWidget::~ACC_AssetMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // not revert() here, the destructor of the model will save the data
    delete mModel; // shared model
    RB_DEBUG->print("ACC_AssetMasterWidget::~ACC_AssetMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_AssetMasterWidget::init() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //
    // 1. Model with ID
    //
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAsset);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    //
    // 2. Relations
    //    mModel->setRelation(mModel->fieldIndex(field),
    //           RB_SqlRelation(relationTable, relationId, relationField));
    mModel->setRelation(mModel->fieldIndex("type_id"),
           RB_SqlRelation("ACC_AssetType", "id", "mname"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    // mModel->setHeaderData(0, Qt::Horizontal, tr("Code"));
    // mModel->setHeaderData(1, Qt::Horizontal, tr("Name"));

    //
    // 4. Connect model to main view, set delegates and hide columns
    //
    setFormatTableView(tv, mModel);

    // Example only
    // tv->setItemDelegateForColumn(mModel->fieldIndex("text"),
    //                                       new RB_TeDelegate(this));

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tv->hideColumn(i);
        } else {
            tv->showColumn(i);

            if (tv->columnWidth(i) < 5) {
                tv->setColumnWidth(i, 100);
            }
        }
    }

    // tv->sortByColumn(-1);
    QApplication::restoreOverrideCursor();
}

/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_AssetMasterWidget::fileSave(bool withSelect) {
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

/**
 * Button add (customer) clicked
 */
void ACC_AssetMasterWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    fileSave(true);
    mModel->undoFilterSort();

    // Make sure corresponding detail MDI window is open for maybeSave()
    RB_MdiWindow* mdiWin = ACC_DIALOGFACTORY->getMdiWindow(
            ACC_DialogFactory::WidgetAsset);
    mdiWin->show();
    ACC_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    QModelIndex idxParent = QModelIndex();
    if (tv->currentIndex().isValid()) {
        idxParent = tv->currentIndex().parent();
    }

    int row = mModel->rowCount(idxParent);
    mModel->insertRows(row, 1, idxParent);

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    setDefaultValues(row, idxParent);
    // end NOTE

    tv->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, idxParent));
    tv->scrollTo(mModel->index(row,  RB2::HIDDENCOLUMNS, idxParent));
    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Set default column values for the relational fields
 * @param row row of the model
 * @param idxParent parent model index
 */
void ACC_AssetMasterWidget::setDefaultValues(int row, const QModelIndex& idxParent) {
    QModelIndex index = mModel->index(row, mModel->fieldIndex("assetcode"), idxParent);
    mModel->setData(index, tr("NEW"), Qt::EditRole);
    QDateTime date = QDateTime::currentDateTime();
    index = mModel->index(row, mModel->fieldIndex("assetsince"), idxParent);
    mModel->setData(index, date, Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("type_id"), idxParent);
    mModel->setData(index, "0", Qt::EditRole);
}

/**
 * Button delete (customer) clicked
 */
void ACC_AssetMasterWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = ACC_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tv->currentIndex();

    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, index.parent());
        fileSave(true);
    }
}

/**
 * Button save clicked
 */
void ACC_AssetMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button set filter clicked
 */
void ACC_AssetMasterWidget::on_pbFilterOn_clicked() {
    filterSourceModel(mModel, "acc_asset", "assetcode", leFilter->text());
}

/**
 * Button clear sourcefilter clicked
 */
void ACC_AssetMasterWidget::on_pbFilterOff_clicked() {
    leFilter->setText("");
    removeFilterSourceModel(mModel);
}


bool ACC_AssetMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
    //    if (maybeSave()) { not here with navigation widgets
    //        return true;
    //    }
    //    return false;
}

/**
 * @returns current selected model index
 */
QModelIndex ACC_AssetMasterWidget::getCurrentModelIndex(int modelType) const {
    if (modelType == ACC_ModelFactory::ModelAsset) {
        return tv->currentIndex();
    }
    return QModelIndex();
}

/**
 * Change event, for example translation
 */
void ACC_AssetMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
