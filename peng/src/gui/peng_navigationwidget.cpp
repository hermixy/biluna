/*****************************************************************
 * $Id: peng_navigationwidget.cpp 2233 2015-04-29 19:10:59Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_navigationwidget.h"

#include "peng_dialogfactory.h"
//#include "peng_equipmentmasterwidget.h"
//#include "peng_linemasterwidget.h"
#include "peng_modelfactory.h"
#include "rb_mdiwindow.h"
//#include "pnid_pnidmasterwidget.h"
//#include "ell_instrumentsmasterwidget.h"

/**
 * Constructor
 */
PENG_NavigationWidget::PENG_NavigationWidget(QWidget *parent) : RB_Widget(parent) {

    setupUi(this);

    mModelEquipment = NULL;
    mModelLine = NULL;

    // old
//    mPnidMasterWidget = NULL;
//    mEquipmentMasterWidget = NULL;
//    mPipeLineMasterWidget = NULL;
//    mInstrumentMasterWidget = NULL;
//    mElectricalMasterWidget = NULL;

    mSizeHint = QSize(200, 600);
}

/**
 * Destructor
 */
PENG_NavigationWidget::~PENG_NavigationWidget() {
    //nothing
}

void PENG_NavigationWidget::init() {
    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();
    leProject->setText(root->getValue("number").toString());

    // For PED tool only:
    tabWidget->setTabEnabled((int)TabPnid, false);

    // Because now opening standard with TabEquipment, will later be TabPnid:
    slotShowTab(TabEquipment);
    tabWidget->setTabEnabled((int)TabInstrument, false);
    tabWidget->setTabEnabled((int)TabElectrical, false);

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotShowTab(int)));

    // equipment model
    //

    // TODO: implement filter
    this->cbEquipmentType->hide();

    // Set model and connect to table view
    mModelEquipment = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipment);
    mModelEquipment->setRoot(PENG_MODELFACTORY->getRootId());

    // Relations, because of separate dockwidget and mdiwidget
    // mModelEquipment->setRelation(mModelEquipment->fieldIndex(field),
    //        QSqlRelation(relationTable, relationId, relationField));

    // Set standard format and connection for model and view
    setFormatTableView(tvEquipment, mModelEquipment);

    // Hide columns
    for (int i = 0; i < mModelEquipment->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS) {
            tvEquipment->hideColumn(i);
        }
    }

    mModelEquipment->select();

    // line model
    //

    // TODO: implement filter
    cbLineType->hide();

    // Set model and connect to table view
    mModelLine = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLine);
    mModelLine->setRoot(PENG_MODELFACTORY->getRootId());

    // Set standard format and connection for model and view
    setFormatTableView(tvLine, mModelLine);

    // Hide columns
    for (int i = 0; i < mModelLine->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS) {
            tvLine->hideColumn(i);
        }
    }

    mModelLine->select();

}

bool PENG_NavigationWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
    if (withSelect) {
        if (mModelLine->submitAllAndSelect()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else {
        if (mModelLine->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;

}


/**
 * Emit state of this widget, i.e. which tab is selected
 */
void PENG_NavigationWidget::emitState() {
    emit currentNavigationTab(tabWidget->currentIndex());
}

/**
 * Show tab and create widgets,
 * called from this navigation widget (user clicked)
 * @param tabType tab type enumerator
 */
void PENG_NavigationWidget::slotShowTab(int tabType) {
    tabWidget->setCurrentIndex(tabType);
//    initTab(tabType);
    emit currentNavigationTab(tabType);
}

void PENG_NavigationWidget::on_pbAddEquipment_clicked() {
    if (!mModelEquipment) return;

    // Make sure corresponding detail MDI window is open for maybeSave()
// TODO:
//    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
//            PENG_DialogFactory::WidgetEquipmentDetail);
//    mdiWin->show();
//    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    if (mModelEquipment->isModelModified()) {
        fileSave(true);
    }

    // Clear sorting and filtering of proxyModel
    mModelEquipment->undoFilterSort();
    int row = 0; // mModelEquipment->rowCount();
    mModelEquipment->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModelEquipment->index(row, mModelEquipment->fieldIndex("number"));
    mModelEquipment->setData(index, "<NEW>", Qt::EditRole);
    index = mModelEquipment->index(row, mModelEquipment->fieldIndex("type_idx"));
    mModelEquipment->setData(index, "0", Qt::EditRole);

    tvEquipment->setCurrentIndex(mModelEquipment->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvEquipment->scrollTo(mModelEquipment->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
}

void PENG_NavigationWidget::on_pbDeleteEquipment_clicked() {
    if (!mModelEquipment) return;

    int res = PENG_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvEquipment->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModelEquipment->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);

        if (row > 0) {
            tvEquipment->setCurrentIndex(mModelEquipment->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvEquipment->scrollTo(mModelEquipment->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

void PENG_NavigationWidget::on_pbSaveEquipment_clicked() {
    fileSave(true);
}

void PENG_NavigationWidget::on_pbAddLine_clicked() {
    if (!mModelLine) return;

    // Make sure corresponding detail MDI window is open for maybeSave()
// TODO:
//    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
//            PENG_DialogFactory::WidgetLineDetail);
//    mdiWin->show();
//    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    if (mModelLine->isModelModified()) {
        fileSave(true);
    }

    // Clear sorting and filtering of proxyModel
    mModelLine->undoFilterSort();
    int row = 0; // mModelLine->rowCount();
    mModelLine->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    // QModelIndex index = mModelLine->index(row, mModelLine->fieldIndex("number"));
    // mModelLine->setData(index, 989100 + i, Qt::EditRole);

    // mModelLine->submitAll(); // not send directly to database because will reload table
    tvLine->setCurrentIndex(mModelLine->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvLine->scrollTo(mModelLine->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
//    mdiWin->getWidget()->slotResetWidgets();
}

void PENG_NavigationWidget::on_pbDeleteLine_clicked() {
    if (!mModelLine) return;

    int res = PENG_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvLine->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModelLine->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);

        if (row > 0) {
            tvLine->setCurrentIndex(mModelLine->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvLine->scrollTo(mModelLine->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

void PENG_NavigationWidget::on_pbSaveLine_clicked() {
    fileSave(true);
}

/**
 * Create tab widgets if not exist
 * @param tabType tab type enumerator
 */
//void PENG_NavigationWidget::setTab(int tabType) {
//    switch (tabType) {
//        case TabPnid:
//        {
//            break;
//        }
//        case TabEquipment:
//        {
////            if (!mEquipmentMasterWidget) {
////                mEquipmentMasterWidget = new PENG_EquipmentMasterWidget(this);
////                mEquipmentMasterWidget->setDialogFactory(PENG_DIALOGFACTORY);
////                mEquipmentMasterWidget->init();

////                QWidget* wdgt = tabWidget->currentWidget();
////                QGridLayout* layout = new QGridLayout(wdgt);
////                layout->setObjectName("layout-equipment");
////                layout->setMargin(0);
////                layout->setSpacing(0);
////                layout->addWidget(mEquipmentMasterWidget, 0, 0, 1, 1);
////            }

//            break;
//        }
//        case TabPipeLine:
//        {
////            if (!mPipeLineMasterWidget) {
////                mPipeLineMasterWidget = new PENG_LineMasterWidget(this);
////                mPipeLineMasterWidget->setDialogFactory(PENG_DIALOGFACTORY);
////                mPipeLineMasterWidget->init();

////                QWidget* wdgt = tabWidget->currentWidget();
////                QGridLayout* layout = new QGridLayout(wdgt);
////                layout->setObjectName("layout-pipeline");
////                layout->setMargin(0);
////                layout->setSpacing(0);
////                layout->addWidget(mPipeLineMasterWidget, 0, 0, 1, 1);
////            }

//            break;
//        }
//        case TabInstrument:
//        {
//            break;
//        }
//        case TabElectrical:
//        {
//            break;
//        }
//        default:
//            break;
//    }
//}

void PENG_NavigationWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
